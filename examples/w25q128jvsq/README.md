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
