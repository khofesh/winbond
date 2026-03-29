/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_w25qxx_interface.c
 * @brief     driver w25qxx interface source file for STM32H533RE + SPI1 DMA
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-07-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/07/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_w25qxx_interface.h"
#include "main.h"
#include <stdarg.h>

extern SPI_HandleTypeDef hspi1;

#define W25QXX_CS_LOW()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define W25QXX_CS_HIGH()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

#define SPI_TIMEOUT_MS  5000

/* 4096 (max sector) + 6 (cmd+addr+dummy) */
#define SPI_BUF_SIZE  4102
static uint8_t gs_tx_buf[SPI_BUF_SIZE];
static uint8_t gs_rx_buf[SPI_BUF_SIZE];

static volatile uint8_t gs_spi_txrx_done = 0;
static volatile uint8_t gs_spi_error = 0;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        gs_spi_txrx_done = 1;
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        gs_spi_error = 1;
    }
}

/**
 * @brief  interface spi qspi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi qspi init failed
 * @note   SPI1 is already initialized by MX_SPI1_Init() in main.c
 */
uint8_t w25qxx_interface_spi_qspi_init(void)
{
    return 0;
}

/**
 * @brief  interface spi qspi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi qspi deinit failed
 * @note   none
 */
uint8_t w25qxx_interface_spi_qspi_deinit(void)
{
    return 0;
}

/**
 * @brief      interface spi qspi bus write read
 * @param[in]  instruction sent instruction
 * @param[in]  instruction_line instruction phy lines
 * @param[in]  address register address
 * @param[in]  address_line address phy lines
 * @param[in]  address_len address length
 * @param[in]  alternate register address
 * @param[in]  alternate_line alternate phy lines
 * @param[in]  alternate_len alternate length
 * @param[in]  dummy dummy cycle
 * @param[in]  *in_buf pointer to a input buffer
 * @param[in]  in_len input length
 * @param[out] *out_buf pointer to a output buffer
 * @param[in]  out_len output length
 * @param[in]  data_line data phy lines
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       Uses a single HAL_SPI_TransmitReceive_DMA for the entire transaction
 *             to avoid STM32H5 SPI peripheral re-enable issues between
 *             separate Transmit/Receive calls.
 */
uint8_t w25qxx_interface_spi_qspi_write_read(uint8_t instruction, uint8_t instruction_line,
                                             uint32_t address, uint8_t address_line, uint8_t address_len,
                                             uint32_t alternate, uint8_t alternate_line, uint8_t alternate_len,
                                             uint8_t dummy, uint8_t *in_buf, uint32_t in_len,
                                             uint8_t *out_buf, uint32_t out_len, uint8_t data_line)
{
    uint32_t total = in_len + out_len;

    if (total == 0)
    {
        return 0;
    }

    if (total > SPI_BUF_SIZE)
    {
        return 1;
    }

    /* Build TX buffer: real data for write phase, 0xFF for read phase */
    if (in_len > 0)
    {
        memcpy(gs_tx_buf, in_buf, in_len);
    }
    if (out_len > 0)
    {
        memset(gs_tx_buf + in_len, 0xFF, out_len);
    }

    W25QXX_CS_LOW();

    gs_spi_txrx_done = 0;
    gs_spi_error = 0;

    if (HAL_SPI_TransmitReceive_DMA(&hspi1, gs_tx_buf, gs_rx_buf, (uint16_t)total) != HAL_OK)
    {
        W25QXX_CS_HIGH();
        return 1;
    }

    uint32_t start = HAL_GetTick();
    while (!gs_spi_txrx_done && !gs_spi_error)
    {
        if ((HAL_GetTick() - start) > SPI_TIMEOUT_MS)
        {
            HAL_SPI_DMAStop(&hspi1);
            W25QXX_CS_HIGH();
            return 1;
        }
    }

    W25QXX_CS_HIGH();

    if (gs_spi_error)
    {
        return 1;
    }

    /* Copy read portion: data clocked in during the out_len phase */
    if (out_len > 0)
    {
        memcpy(out_buf, gs_rx_buf + in_len, out_len);
    }

    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void w25qxx_interface_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
 * @brief     interface delay us
 * @param[in] us time
 * @note      none
 */
void w25qxx_interface_delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < ticks)
    {
    }
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void w25qxx_interface_debug_print(const char *const fmt, ...)
{
    char buf[256];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    printf("%s", buf);
}
