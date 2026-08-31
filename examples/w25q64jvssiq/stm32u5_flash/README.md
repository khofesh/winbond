# winbond flash + STM32U585CIU6

W25Q64JV SPI NOR flash on a WeAct STM32U585CIU6 board, driven over SPI1 with GPDMA1.
Ported from the [`w25q128jvsq`](../../w25q128jvsq) example (Nucleo-H533RE), which carries the longer
write-up on the GPDMA1 setup and SPI troubleshooting.

## flash pin connection

- F_CS -> PA4
- SCK -> PA5
- MOSI -> PA7
- MISO -> PA6

Tie /WP and /HOLD to 3.3V — floating them causes unpredictable behaviour.
LED (USER_LED) is PC13.

## SPI configuration

- SPI1, full-duplex master, software NSS (CS driven manually on PA4)
- CPOL=Low, CPHA=1Edge (SPI mode 0), 8-bit, MSB first
- FIFO threshold 8 data
- SPI1 kernel clock 160 MHz, `BaudRatePrescaler = 8` → **20 MHz SCK**

> Keep SCK ≤ 50 MHz. The driver's SPI read path issues `0x03` Read Data, which the W25Q64JV specs at
> 50 MHz max; the 133 MHz figure applies only to `0x0B` Fast Read.

## GPDMA1

| Channel | Request | Direction | SrcInc | DestInc | Link |
| ------- | ------- | --------- | ------ | ------- | ---- |
| 0 | `GPDMA1_REQUEST_SPI1_TX` | `DMA_MEMORY_TO_PERIPH` | `SINC_INCREMENTED` | `DINC_FIXED` | `hdmatx` |
| 1 | `GPDMA1_REQUEST_SPI1_RX` | `DMA_PERIPH_TO_MEMORY` | `SINC_FIXED` | `DINC_INCREMENTED` | `hdmarx` |

Both channels are byte width, normal mode, block-transfer event mode. NVIC needs GPDMA1 Channel 0,
GPDMA1 Channel 1 and SPI1 global interrupts enabled.

> **CubeMX pitfall:** CubeMX has generated *both* channels as `SPI1_TX` with fixed increments and both
> linked to `hdmatx`. Re-check `Core/Src/stm32u5xx_hal_msp.c` after every code generation.

`MX_GPDMA1_Init()` must run before `MX_SPI1_Init()` — SPI init triggers `HAL_SPI_MspInit()`, which
configures the DMA channels, so the GPDMA1 clock has to be on already.

## driver layers

```
main.c
  └── driver_w25qxx_basic.c / driver_w25qxx_advance.c   (Core/Src, LibDriver examples)
        └── Drivers/W25QXX/driver_w25qxx.c              (vendored core driver, unmodified)
              └── driver_w25qxx_interface.c             (Core/Src, board layer)
```

`driver_w25qxx_interface.c` provides:

- `w25qxx_interface_spi_qspi_write_read()` — one `HAL_SPI_TransmitReceive_DMA()` per transaction
  (not separate Transmit + Receive), CS low around the whole transfer, 5 s timeout, completion
  signalled by `HAL_SPI_TxRxCpltCallback` / `HAL_SPI_ErrorCallback`
- `w25qxx_interface_delay_us()` — DWT cycle counter. **`DWT->CYCCNT` is disabled out of reset**, so the
  function enables `DCB->DEMCR TRCENA` and `DWT->CTRL CYCCNTENA` on first call; without that the wait
  loop never terminates. It appears to work under a debugger, which enables TRCENA itself, and hangs
  on a standalone boot.
- `w25qxx_interface_debug_print()` — `printf`, but no UART is configured, so output reaches the `_write`
  stub in `syscalls.c` and is discarded.

`SPI_BUF_SIZE` (4102 = 4096 sector + 6 cmd/addr/dummy) caps a single transfer; a `w25qxx_basic_read()`
longer than that returns 1.

## self test

`main()` runs `flash_self_test()`:

1. `w25qxx_basic_init(W25Q64, W25QXX_INTERFACE_SPI, W25QXX_BOOL_FALSE)`
2. `w25qxx_basic_get_id()` — expects manufacturer `0xEF`, device `0x16`
3. writes a 256-byte pattern at address `0x000000` (this erases the containing 4 KB sector)
4. reads it back and compares

Pass → PC13 blinks at 200 ms. Fail → PC13 held low.
