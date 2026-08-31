# claude

## Project

STM32CubeIDE project `stm32u5_flash`: WeAct STM32U585CIU6 (UFQFPN48, TrustZone disabled) driving a
Winbond W25Q64JV SPI NOR flash. Part of the `winbond` repo; the sibling example
`examples/w25q128jvsq` (Nucleo-H533RE + W25Q128) is the reference implementation — its `README.md`
documents the SPI/GPDMA1 setup and troubleshooting, and its `Core/Src/driver_w25qxx_{basic,advance,interface}.c`
are the pattern to follow when porting here.

Layout:

- `Core/Src`, `Core/Inc` — CubeMX-generated app code (`main.c`, `stm32u5xx_it.c`, `stm32u5xx_hal_msp.c`)
- `Drivers/W25QXX` — LibDriver `driver_w25qxx.{c,h}`, vendored upstream. Do not modify; write the
  platform layer (`driver_w25qxx_interface.c`: SPI read/write, GPIO CS, delay, debug print) instead
- `Drivers/STM32U5xx_HAL_Driver`, `Drivers/CMSIS` — vendor code, never edit
- `stm32u5_flash.ioc` — CubeMX config; `STM32U585CIUX_FLASH.ld` / `_RAM.ld` — linker scripts
- Build via STM32CubeIDE (GCC ARM); no Makefile checked in for this example

Hardware wiring (SPI1, software NSS):

| W25Q64JV | STM32U585 |
| -------- | --------- |
| /CS      | PA4       |
| CLK      | PA5       |
| DO/MISO  | PA6       |
| DI/MOSI  | PA7       |

Tie /WP and /HOLD to 3.3V.

Current state: CubeMX skeleton only — `main.c` has empty USER CODE blocks and the W25QXX driver is not
yet wired up. GPDMA1 Channel0 (SPI1 TX) and Channel1 (intended SPI1 RX) are generated, but Channel1 is
still wrong out of CubeMX: it uses `GPDMA1_REQUEST_SPI1_TX`, `DMA_SINC_FIXED`/`DMA_DINC_FIXED` and links
to `hdmatx`. RX needs `GPDMA1_REQUEST_SPI1_RX`, `DMA_PERIPH_TO_MEMORY`, `DMA_DINC_INCREMENTED` and
`__HAL_LINKDMA(hspi, hdmarx, ...)`; TX needs `DMA_SINC_INCREMENTED`. Fix it in CubeMX, not by hand-editing
generated code.

## STM32CubeMX

- if you need to change the configuration in stm32cubemx, don't make changes in `*.ioc` file. Show step-by-step guide instead

## Response Style

- Be extremely concise. No pleasantries, no filler.
- When asked to write code, return code only unless explanation is explicitly requested.
- No sycophantic preambles ("Sure!", "Great question!", "Absolutely!").
- No "Here's a function that..." preambles.
- Don't restate the question before answering.
- No "Note:", "Tip:", or "Remember:" appendices unless asked.
- No usage examples unless asked.
- No unsolicited suggestions or improvements beyond what was asked.
- Use short variable names where meaning is clear from context.

## Code Style

- C (C11), STM32CubeIDE / GCC ARM toolchain, STM32U5 HAL drivers
- Never write outside the `/* USER CODE BEGIN X */` ... `/* USER CODE END X */` markers in CubeMX-generated files — code outside is lost on regeneration
- Application code goes in `Core/Src` and `Core/Inc`; reusable drivers in their own `.c`/`.h` pair under `Drivers/`
- Match the surrounding file's style: CubeMX files use CubeMX conventions (`__FILE_H` guards, doxygen block comments); LibDriver files use LibDriver conventions (`DRIVER_W25QXX_H` guards, `w25qxx_` prefix, `uint8_t` status returns where 0 is success)
- Use fixed-width types from `<stdint.h>` (`uint8_t`, `int32_t`), not `int`/`long`
- `static` for file-local functions and variables; no globals unless shared across translation units
- Peripheral handles (`hspi1`, `handle_GPDMA1_Channel0`, `handle_GPDMA1_Channel1`) come from CubeMX — declare with `extern` where needed, never redefine
- Check and handle `HAL_StatusTypeDef` return values; don't ignore them
- No dynamic allocation (`malloc`/`free`); use static buffers with explicit sizes
- Keep ISRs short — set flags, defer work to the main loop; shared ISR variables must be `volatile`
- No blocking `HAL_Delay()` in ISRs or long-running paths; prefer non-blocking / DMA / interrupt-driven APIs
- SPI CS is software-controlled on PA4 — drive it low before a transfer and high after, including on error paths
- Minimal inline comments — only for genuinely complex logic or hardware quirks (register timing, errata)

## Context Retrieval Policy

Always retrieve the smallest amount of information necessary.
Escalate only when necessary. Stop escalating as soon as sufficient information has been obtained.

Preferred order:

1. Need code structure? `graph.json` (if `graphify-out/` exists).
2. Need symbols? `ast-grep`.
3. Need repository metadata? `rtk`.
4. Need implementation? Source files.
5. Use repository-wide search as last resort

Avoid reading entire directories or the whole repository unless explicitly requested. `driver_w25qxx.c` is
~8.5k lines — locate the symbol first, then read only that region. Never scan `Drivers/STM32U5xx_HAL_Driver`
or `Drivers/CMSIS`; consult the HAL headers in `Drivers/STM32U5xx_HAL_Driver/Inc` for API signatures.

## RTK (Rust Token Killer) - Token-Optimized CLI

`rtk` is a CLI proxy that filters and compresses command outputs, saving 60-90% tokens.

**Always** prefix commands with `rtk`. If RTK has a dedicated filter, it uses it. If not, it passes through unchanged. This means RTK is always safe to use.

Instead of:

```
ls -la .
git status
git log -10
docker ps
```

Use:

```
rtk ls -la .
rtk git status
rtk git log -10
rtk docker ps
```

### RTK commands example

```bash
rtk ls <path>
rtk read <file>
rtk find <pattern>

rtk err <cmd>           # Filter errors only from any command
rtk log <file>          # Deduplicated logs with counts
rtk json <file>         # JSON structure without values

rtk curl <url>          # Compact HTTP responses

rtk docker ps           # Compact container list
rtk docker images       # Compact image list
rtk docker logs <c>     # Deduplicated logs
```

## Graphify - Codebase context & knowledge graph protocol

If a pre-computed AST knowledge graph exists at `graphify-out/graph.json`, use it before searching or
reading multiple source files. It is not committed — if the directory is absent, skip this section and use
`ast-grep` instead. `.graphifyignore` excludes vendor HAL/CMSIS code, `Debug/`, images and docs.

Workflow:

1. Read `graph.json`.
2. Identify the relevant symbols, files, and dependency paths.
3. Read only the source files required for the task.
4. Avoid scanning unrelated files.

Use the graph for:

- dependency tracing
- call-path discovery
- identifying high-centrality modules
- impact analysis before refactoring
- locating symbol definitions

Never read an entire directory simply to locate a symbol. Use `graph.json` to locate the symbol first, then read only the relevant files.

The graph is generated from AST extraction and represents extracted structural relationships only. Treat graph edges as authoritative for structural relationships.
Do not infer dependencies that are absent from the graph.

## ast-grep

Prefer `ast-grep` over `grep` whenever searching C source (`-l c` for `.c`/`.h`).

Use `ast-grep` for:

- locating function definitions
- locating struct/typedef definitions
- locating call sites of a HAL or driver function
- finding includes
- matching AST patterns
- performing structural search

Use `grep` only for:

- Markdown
- JSON
- YAML
- log files
- generated text
- plain-text configuration
- `.ioc`, `.cproject`, `.mxproject` and linker scripts

Avoid using `grep` to search programming language source code when `ast-grep` can answer the query.
