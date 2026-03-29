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

## driver_w25qxx_interface.c implementation

The interface uses a **single `HAL_SPI_TransmitReceive_IT`** call per transaction with static TX/RX buffers (4102 bytes each). This is critical for the STM32H5 SPI peripheral — see the troubleshooting notes below.

## troubleshooting

### STM32H5 SPI: separate Transmit + Receive fails

**Problem:** Using `HAL_SPI_Transmit` followed by `HAL_SPI_Receive` (or their `_IT` variants) within a single CS assertion fails on STM32H5, even though CS stays low the entire time.

**Root cause:** The STM32H5 SPI peripheral (SPI v2/v3) disables SPE (SPI Enable) at the end of each HAL transfer call. When SPE is cleared and re-set between the transmit and receive phases, the peripheral resets its internal state. This corrupts the transaction with the flash chip, which expects continuous clocking under a single CS assertion.

**Solution:** Use a single `HAL_SPI_TransmitReceive` (or `_IT`) call for the entire transaction. Build a combined TX buffer:

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

**Alternative:** Use `SPI_BAUDRATEPRESCALER_4` or higher to slow the SPI clock, or use DMA instead of interrupt mode for large transfers.
