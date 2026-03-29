# w25q128jvsq + nucleo-h533re

## connections

## Nucleo-H533RE → W25Q128 Connection

| W25Q128 (DIP Pin) | Signal    | Nucleo Pin                | STM32 GPIO |
| ----------------- | --------- | ------------------------- | ---------- |
| 1                 | /CS       | CN10 pin 15 (or any GPIO) | PB6        |
| 2                 | DO (MISO) | CN10 pin 13               | PA6        |
| 3                 | /WP       | 3.3V (tie high)           | -          |
| 4                 | GND       | CN7 pin 8 (GND)           | GND        |
| 5                 | DI (MOSI) | CN10 pin 15               | PA7        |
| 6                 | CLK       | CN10 pin 11               | PA5        |
| 7                 | /HOLD     | 3.3V (tie high)           | -          |
| 8                 | VCC       | CN7 pin 16 (3.3V)         | 3.3V       |

> **Note:** /WP and /HOLD should be tied to 3.3V to keep them inactive (not floating). This is important — floating these pins can cause unexpected behavior.

For CS, connected to **PB6** and managed in software rather than hardware NSS, since software CS gives you more control and is easier to debug.

## SPI configuration

- SPI1, full-duplex master, software NSS
- CPOL=Low, CPHA=1Edge (SPI Mode 0)
- 8-bit data size
- MSB first
- FIFO threshold: **8 data** (see [troubleshooting](#spi-overrun-in-interrupt-mode))

## DMA setup (GPDMA1) — step by step

This section documents how to configure GPDMA1 in STM32CubeMX for SPI1 DMA transfers on the STM32H533RE.

### Step 1: Enable GPDMA1 in CubeMX

1. Open `w25q128jvsq.ioc` in STM32CubeMX
2. Go to **Pinout & Configuration** → **System Core** → **GPDMA1**
3. Enable **Channel 0** (for SPI1_TX) and **Channel 1** (for SPI1_RX)

### Step 2: Configure Channel 0 — SPI1 TX

| Setting                 | Value                                    |
| ----------------------- | ---------------------------------------- |
| Request                 | `GPDMA1_REQUEST_SPI1_TX`                 |
| Direction               | **Memory to Peripheral**                 |
| Source Address Inc      | **Incremented** (`DMA_SINC_INCREMENTED`) |
| Destination Address Inc | **Fixed** (`DMA_DINC_FIXED`)             |
| Source Data Width       | Byte                                     |
| Destination Data Width  | Byte                                     |
| Mode                    | Normal                                   |

> **CubeMX pitfall:** CubeMX may generate `DMA_PERIPH_TO_MEMORY` and `DMA_SINC_FIXED` for TX. This is wrong — TX must be `DMA_MEMORY_TO_PERIPH` with `DMA_SINC_INCREMENTED`. Verify the generated `stm32h5xx_hal_msp.c` after code generation.

### Step 3: Configure Channel 1 — SPI1 RX

| Setting                 | Value                                    |
| ----------------------- | ---------------------------------------- |
| Request                 | `GPDMA1_REQUEST_SPI1_RX`                 |
| Direction               | **Peripheral to Memory**                 |
| Source Address Inc      | **Fixed** (`DMA_SINC_FIXED`)             |
| Destination Address Inc | **Incremented** (`DMA_DINC_INCREMENTED`) |
| Source Data Width       | Byte                                     |
| Destination Data Width  | Byte                                     |
| Mode                    | Normal                                   |

> **CubeMX pitfall:** CubeMX may generate `DMA_DINC_FIXED` for RX. This is wrong — RX destination must be `DMA_DINC_INCREMENTED`, otherwise every received byte overwrites the same address and you only get the last byte.

### Step 4: Enable NVIC interrupts

In CubeMX **NVIC** settings, ensure these are enabled:

- **GPDMA1 Channel 0 global interrupt** — for SPI1 TX DMA completion
- **GPDMA1 Channel 1 global interrupt** — for SPI1 RX DMA completion
- **SPI1 global interrupt** — for SPI error handling

All three are required. Without the SPI1 interrupt, DMA error conditions won't be handled properly.

### Step 5: Generate code and verify

After generating code, verify `stm32h5xx_hal_msp.c` contains the correct DMA config:

```c
/* SPI1 RX — Channel 1 */
handle_GPDMA1_Channel1.Init.Direction = DMA_PERIPH_TO_MEMORY;
handle_GPDMA1_Channel1.Init.SrcInc = DMA_SINC_FIXED;
handle_GPDMA1_Channel1.Init.DestInc = DMA_DINC_INCREMENTED;  /* NOT DMA_DINC_FIXED */

/* SPI1 TX — Channel 0 */
handle_GPDMA1_Channel0.Init.Direction = DMA_MEMORY_TO_PERIPH; /* NOT DMA_PERIPH_TO_MEMORY */
handle_GPDMA1_Channel0.Init.SrcInc = DMA_SINC_INCREMENTED;    /* NOT DMA_SINC_FIXED */
handle_GPDMA1_Channel0.Init.DestInc = DMA_DINC_FIXED;
```

Also verify that the DMA channels are linked to SPI1:

```c
__HAL_LINKDMA(hspi, hdmarx, handle_GPDMA1_Channel1);
__HAL_LINKDMA(hspi, hdmatx, handle_GPDMA1_Channel0);
```

### Step 6: Verify interrupt handlers

In `stm32h5xx_it.c`, CubeMX should have generated these three handlers:

```c
void GPDMA1_Channel0_IRQHandler(void)  /* TX DMA */
{
    HAL_DMA_IRQHandler(&handle_GPDMA1_Channel0);
}

void GPDMA1_Channel1_IRQHandler(void)  /* RX DMA */
{
    HAL_DMA_IRQHandler(&handle_GPDMA1_Channel1);
}

void SPI1_IRQHandler(void)             /* SPI error handling */
{
    HAL_SPI_IRQHandler(&hspi1);
}
```

### Step 7: Verify GPDMA1 clock and NVIC in main.c

`MX_GPDMA1_Init()` should enable the GPDMA1 clock and configure NVIC:

```c
static void MX_GPDMA1_Init(void)
{
    __HAL_RCC_GPDMA1_CLK_ENABLE();

    HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
}
```

> **Important:** `MX_GPDMA1_Init()` must be called **before** `MX_SPI1_Init()` in `main()`, because SPI init triggers `HAL_SPI_MspInit` which configures the DMA channels. The DMA clock must already be enabled at that point.

### Step 8: Use HAL_SPI_TransmitReceive_DMA in the interface

In `driver_w25qxx_interface.c`, use `HAL_SPI_TransmitReceive_DMA` with completion callbacks:

```c
static volatile uint8_t gs_spi_txrx_done = 0;
static volatile uint8_t gs_spi_error = 0;

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
        gs_spi_txrx_done = 1;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
        gs_spi_error = 1;
}
```

Then in the transfer function:

```c
gs_spi_txrx_done = 0;
gs_spi_error = 0;

HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buf, rx_buf, total);

while (!gs_spi_txrx_done && !gs_spi_error)
{
    /* wait — CPU is free to do other work here */
}
```

> **Key point:** Use a single `HAL_SPI_TransmitReceive_DMA` call per transaction (not separate Transmit + Receive). See [troubleshooting](#stm32h5-spi-separate-transmit--receive-fails) for why.

## driver_w25qxx_interface.c implementation

The interface uses a **single `HAL_SPI_TransmitReceive_DMA`** call per transaction with static TX/RX buffers (4102 bytes each). This is critical for the STM32H5 SPI peripheral — see the troubleshooting notes below.

## troubleshooting

### STM32H5 SPI: separate Transmit + Receive fails

**Problem:** Using `HAL_SPI_Transmit` followed by `HAL_SPI_Receive` (or their `_IT` / `_DMA` variants) within a single CS assertion fails on STM32H5, even though CS stays low the entire time.

**Root cause:** The STM32H5 SPI peripheral (SPI v2/v3) disables SPE (SPI Enable) at the end of each HAL transfer call. When SPE is cleared and re-set between the transmit and receive phases, the peripheral resets its internal state. This corrupts the transaction with the flash chip, which expects continuous clocking under a single CS assertion.

**Solution:** Use a single `HAL_SPI_TransmitReceive` (or `_IT` / `_DMA`) call for the entire transaction. Build a combined TX buffer:

```
TX: [command + address + dummy bytes] [0xFF padding for read clocks]
RX: [garbage during cmd phase]        [actual read data]
```

Then copy `rx_buf[in_len .. in_len+out_len]` to the output buffer. This requires static buffers large enough for the largest transaction (4096-byte sector read + 6 bytes overhead = 4102 bytes).

### SPI overrun in interrupt mode

**Problem:** `HAL_SPI_TransmitReceive_IT` triggers `HAL_SPI_ERROR_OVR` (overrun error, code `0x00000004`) on transfers larger than a few bytes.

**Root cause:** With `SPI_FIFO_THRESHOLD_01DATA`, the SPI interrupt fires for every single byte. At the default SPI clock (~21 MHz with `SPI_BAUDRATEPRESCALER_2` from PLL1Q), one byte completes in ~380ns. The Cortex-M33 interrupt entry/exit latency plus the `HAL_SPI_IRQHandler` processing time exceeds this, so the RX FIFO overflows before the ISR can drain it.

The `get_manufacturer_device_id` command (6 bytes total) worked because the transfer was short enough to complete before the FIFO overflowed. Larger transfers (fast read with 21+ bytes) consistently failed.

**Solution:** Increase the FIFO threshold in `MX_SPI1_Init`:

```c
// Before (fails with overrun):
hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;

// After (works):
hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_08DATA;
```

With 8-byte threshold, interrupts fire 8x less frequently, giving the ISR ample time to process data. The STM32H5 SPI FIFO is 16 bytes deep (for 8-bit frames), so an 8-byte threshold provides good balance between interrupt frequency and FIFO headroom.

**Alternative:** Use `SPI_BAUDRATEPRESCALER_4` or higher to slow the SPI clock, or use DMA instead of interrupt mode for large transfers (DMA does not have this overrun issue since the hardware handles data movement without CPU intervention).

### CubeMX generates wrong DMA direction/increment settings

**Problem:** After configuring GPDMA1 for SPI1 in CubeMX and generating code, the SPI DMA transfers silently produce wrong data or fail.

**Root cause:** CubeMX may generate incorrect settings in `stm32h5xx_hal_msp.c`:

- TX channel: `DMA_PERIPH_TO_MEMORY` instead of `DMA_MEMORY_TO_PERIPH`, and `DMA_SINC_FIXED` instead of `DMA_SINC_INCREMENTED`
- RX channel: `DMA_DINC_FIXED` instead of `DMA_DINC_INCREMENTED`

**Solution:** After every code generation, verify the DMA settings in `stm32h5xx_hal_msp.c` match:

| Channel  | Direction              | SrcInc                 | DestInc                |
| -------- | ---------------------- | ---------------------- | ---------------------- |
| TX (CH0) | `DMA_MEMORY_TO_PERIPH` | `DMA_SINC_INCREMENTED` | `DMA_DINC_FIXED`       |
| RX (CH1) | `DMA_PERIPH_TO_MEMORY` | `DMA_SINC_FIXED`       | `DMA_DINC_INCREMENTED` |
