# Graph Report - .  (2026-08-31)

## Corpus Check
- 9 files · ~59,313 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 2170 nodes · 4846 edges · 53 communities (50 shown, 3 thin omitted)
- Extraction: 98% EXTRACTED · 2% INFERRED · 0% AMBIGUOUS · INFERRED: 112 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- LL RCC Clock Registers
- LL GPDMA Register Layer
- LL SPI Register Layer
- HAL DMA Driver
- LL Power Control A
- LL Power Control B
- LL Bus Clock Gating
- W25QXX Core Driver
- LL System & Debug A
- LL Debug Freeze B
- HAL SPI Driver
- HAL DMA Linked Lists
- HAL Internal Flash
- LL Cortex-M33 Core
- LL Clock Recovery CRS
- HAL Core & Tick
- W25QXX Advance API
- LL Instruction Cache
- LL GPIO Registers
- LL EXTI Lines
- HAL Cortex MPU/NVIC
- HAL TrustZone GTZC
- HAL ICACHE Driver
- HAL RCC Clock Config
- HAL GPIO Driver
- HAL Power Driver
- HAL RCC CRS Extended
- Newlib Syscall Stubs
- Application Init & MSP
- HAL EXTI Driver
- HAL RCC PLL Extended
- Interrupt Handlers
- HAL Voltage Scaling
- HAL Init & Systick
- HAL I2C Extended
- HAL SPI Autonomous Mode
- Driver Init Entry Points
- W25QXX SPI Interface Layer
- LL Device Utilities
- HAL Voltage Monitor Callbacks
- HAL PLL Frequency Query
- HAL CRS Callbacks
- HAL Backup Domain Access
- W25QXX Burst Wrap

## God Nodes (most connected - your core abstractions)
1. `a_w25qxx_qspi_write_read()` - 57 edges
2. `a_w25qxx_spi_write_read()` - 44 edges
3. `HAL_GetTick()` - 43 edges
4. `I2C_Enable_IRQ()` - 29 edges
5. `I2C_TransferConfig()` - 25 edges
6. `DMA_List_GetCLLRNodeInfo()` - 24 edges
7. `HAL_DMA_Start_IT()` - 19 edges
8. `HAL_DMAEx_List_Start_IT()` - 19 edges
9. `DMA_List_FindNode()` - 19 edges
10. `I2C_ITError()` - 19 edges

## Surprising Connections (you probably didn't know these)
- `SPI1_IRQHandler()` --calls--> `HAL_SPI_IRQHandler()`  [INFERRED]
  Core/Src/stm32u5xx_it.c → Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_spi.c
- `SysTick_Handler()` --calls--> `HAL_IncTick()`  [INFERRED]
  Core/Src/stm32u5xx_it.c → Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal.c
- `GPDMA1_Channel0_IRQHandler()` --calls--> `HAL_DMA_IRQHandler()`  [INFERRED]
  Core/Src/stm32u5xx_it.c → Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dma.c
- `GPDMA1_Channel1_IRQHandler()` --calls--> `HAL_DMA_IRQHandler()`  [INFERRED]
  Core/Src/stm32u5xx_it.c → Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_dma.c
- `HAL_RCCEx_CRSWaitSynchronization()` --calls--> `HAL_GetTick()`  [INFERRED]
  Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal_rcc_ex.c → Drivers/STM32U5xx_HAL_Driver/Src/stm32u5xx_hal.c

## Import Cycles
- None detected.

## Communities (53 total, 3 thin omitted)

### Community 0 - "LL RCC Clock Registers"
Cohesion: 0.01
Nodes (332): __STATIC_INLINE, LL_RCC_ClearFlag_HSECSS(), LL_RCC_ClearFlag_HSERDY(), LL_RCC_ClearFlag_HSI48RDY(), LL_RCC_ClearFlag_HSIRDY(), LL_RCC_ClearFlag_LSERDY(), LL_RCC_ClearFlag_LSIRDY(), LL_RCC_ClearFlag_MSIKRDY() (+324 more)

### Community 1 - "LL GPDMA Register Layer"
Cohesion: 0.03
Nodes (169): DMA_TypeDef, __STATIC_INLINE, LL_DMA_ClearFlag_DTE(), LL_DMA_ClearFlag_HT(), LL_DMA_ClearFlag_SUSP(), LL_DMA_ClearFlag_TC(), LL_DMA_ClearFlag_TO(), LL_DMA_ClearFlag_ULE() (+161 more)

### Community 2 - "LL SPI Register Layer"
Cohesion: 0.04
Nodes (148): __STATIC_INLINE, LL_SPI_ClearFlag_CRCERR(), LL_SPI_ClearFlag_EOT(), LL_SPI_ClearFlag_FRE(), LL_SPI_ClearFlag_MODF(), LL_SPI_ClearFlag_OVR(), LL_SPI_ClearFlag_SUSP(), LL_SPI_ClearFlag_TXTF() (+140 more)

### Community 3 - "HAL DMA Driver"
Cohesion: 0.07
Nodes (118): DMA_HandleTypeDef, HAL_StatusTypeDef, DMA_Init(), DMA_SetConfig(), HAL_DMAEx_List_Start_IT(), HAL_DMA_Abort(), HAL_DMA_Abort_IT(), HAL_DMA_ConfigChannelAttributes() (+110 more)

### Community 4 - "LL Power Control A"
Cohesion: 0.02
Nodes (96): LL_PWR_ClearFlag_SB(), LL_PWR_ClearFlag_STOP(), LL_PWR_ClearFlag_WU1(), LL_PWR_ClearFlag_WU3(), LL_PWR_ClearFlag_WU6(), LL_PWR_ClearFlag_WU7(), LL_PWR_DisableBatteryCharging(), LL_PWR_DisableEPODBooster() (+88 more)

### Community 5 - "LL Power Control B"
Cohesion: 0.02
Nodes (97): __STATIC_INLINE, LL_PWR_ClearFlag_WU(), LL_PWR_ClearFlag_WU2(), LL_PWR_ClearFlag_WU4(), LL_PWR_ClearFlag_WU5(), LL_PWR_ClearFlag_WU8(), LL_PWR_ConfigSecure(), LL_PWR_DisableBkUpAccess() (+89 more)

### Community 6 - "LL Bus Clock Gating"
Cohesion: 0.05
Nodes (82): __STATIC_INLINE, LL_AHB1_GRP1_DisableBusClock(), LL_AHB1_GRP1_DisableClock(), LL_AHB1_GRP1_DisableClockStopSleep(), LL_AHB1_GRP1_EnableBusClock(), LL_AHB1_GRP1_EnableClock(), LL_AHB1_GRP1_EnableClockStopSleep(), LL_AHB1_GRP1_ForceReset() (+74 more)

### Community 7 - "W25QXX Core Driver"
Cohesion: 0.08
Nodes (77): w25qxx_advance_only_qspi_set_read_parameters(), a_w25qxx_erase_sector(), a_w25qxx_page_program(), a_w25qxx_qspi_write_read(), a_w25qxx_read(), a_w25qxx_spi_write_read(), a_w25qxx_write_no_check(), w25qxx_block_erase_32k() (+69 more)

### Community 8 - "LL System & Debug A"
Cohesion: 0.03
Nodes (69): LL_DBGMCU_APB1_GRP1_FreezePeriph(), LL_DBGMCU_APB1_GRP2_FreezePeriph(), LL_DBGMCU_APB2_GRP1_FreezePeriph(), LL_DBGMCU_APB2_GRP1_UnFreezePeriph(), LL_DBGMCU_APB3_GRP1_FreezePeriph(), LL_DBGMCU_DisableDBGStopMode(), LL_DBGMCU_EnableDBGStandbyMode(), LL_DBGMCU_GetDeviceID() (+61 more)

### Community 9 - "LL Debug Freeze B"
Cohesion: 0.03
Nodes (70): __STATIC_INLINE, LL_DBGMCU_AHB1_GRP1_FreezePeriph(), LL_DBGMCU_AHB1_GRP1_UnFreezePeriph(), LL_DBGMCU_AHB3_GRP1_FreezePeriph(), LL_DBGMCU_AHB3_GRP1_UnFreezePeriph(), LL_DBGMCU_APB1_GRP1_UnFreezePeriph(), LL_DBGMCU_APB1_GRP2_UnFreezePeriph(), LL_DBGMCU_APB3_GRP1_UnFreezePeriph() (+62 more)

### Community 10 - "HAL SPI Driver"
Cohesion: 0.09
Nodes (61): HAL_DMA_GetError(), DMA_HandleTypeDef, FlagStatus, HAL_StatusTypeDef, SPI_HandleTypeDef, __weak, HAL_SPI_Abort(), HAL_SPI_Abort_IT() (+53 more)

### Community 11 - "HAL DMA Linked Lists"
Cohesion: 0.13
Nodes (57): DMA_DataHandlingConfTypeDef, DMA_NodeConfTypeDef, DMA_NodeInQInfoTypeDef, DMA_NodeTypeDef, DMA_QListTypeDef, DMA_RepeatBlockConfTypeDef, DMA_TriggerConfTypeDef, DMA_HandleTypeDef (+49 more)

### Community 12 - "HAL Internal Flash"
Cohesion: 0.07
Nodes (46): HAL_StatusTypeDef, __weak, HAL_StatusTypeDef, FLASH_MassErase(), FLASH_OB_BootAddrConfig(), FLASH_OB_BootLockConfig(), FLASH_OB_GetBootAddr(), FLASH_OB_GetBootLock() (+38 more)

### Community 13 - "LL Cortex-M33 Core"
Cohesion: 0.08
Nodes (50): __STATIC_INLINE, LL_CPUID_GetArchitecture(), LL_CPUID_GetImplementer(), LL_CPUID_GetParNo(), LL_CPUID_GetRevision(), LL_CPUID_GetVariant(), LL_HANDLER_DisableFault(), LL_HANDLER_EnableFault() (+42 more)

### Community 15 - "LL Clock Recovery CRS"
Cohesion: 0.08
Nodes (46): __STATIC_INLINE, LL_CRS_ClearFlag_ERR(), LL_CRS_ClearFlag_ESYNC(), LL_CRS_ClearFlag_SYNCOK(), LL_CRS_ClearFlag_SYNCWARN(), LL_CRS_ConfigSynchronization(), LL_CRS_DisableAutoTrimming(), LL_CRS_DisableFreqErrorCounter() (+38 more)

### Community 16 - "HAL Core & Tick"
Cohesion: 0.05
Nodes (9): HAL_StatusTypeDef, HAL_DeInit(), HAL_GetTickFreq(), HAL_MspDeInit(), HAL_SetTickFreq(), HAL_SYSCFG_EnableVREFBUF(), HAL_SYSCFG_GetConfigAttributes(), HAL_SYSCFG_GetLock() (+1 more)

### Community 18 - "LL Instruction Cache"
Cohesion: 0.10
Nodes (37): __STATIC_INLINE, LL_ICACHE_ClearFlag_BSYEND(), LL_ICACHE_ClearFlag_ERR(), LL_ICACHE_Disable(), LL_ICACHE_DisableIT_BSYEND(), LL_ICACHE_DisableIT_ERR(), LL_ICACHE_DisableMonitors(), LL_ICACHE_DisableRegion() (+29 more)

### Community 19 - "LL GPIO Registers"
Cohesion: 0.18
Nodes (31): GPIO_TypeDef, __STATIC_INLINE, LL_GPIO_DisableHighSPeedLowVoltage(), LL_GPIO_DisablePinSecure(), LL_GPIO_EnableHighSPeedLowVoltage(), LL_GPIO_EnablePinSecure(), LL_GPIO_GetAFPin_0_7(), LL_GPIO_GetAFPin_8_15() (+23 more)

### Community 20 - "LL EXTI Lines"
Cohesion: 0.12
Nodes (30): __STATIC_INLINE, LL_EXTI_ClearFallingFlag_0_31(), LL_EXTI_ClearRisingFlag_0_31(), LL_EXTI_DisableEvent_0_31(), LL_EXTI_DisableFallingTrig_0_31(), LL_EXTI_DisableIT_0_31(), LL_EXTI_DisablePrivilege_0_31(), LL_EXTI_DisableRisingTrig_0_31() (+22 more)

### Community 21 - "HAL Cortex MPU/NVIC"
Cohesion: 0.10
Nodes (20): __weak, HAL_MPU_ConfigMemoryAttributes(), HAL_MPU_ConfigMemoryAttributes_NS(), HAL_MPU_ConfigRegion(), HAL_MPU_ConfigRegion_NS(), HAL_NVIC_ClearPendingIRQ(), HAL_NVIC_DisableIRQ(), HAL_NVIC_EnableIRQ() (+12 more)

### Community 22 - "HAL TrustZone GTZC"
Cohesion: 0.12
Nodes (27): HAL_StatusTypeDef, __weak, HAL_GTZC_IRQHandler(), HAL_GTZC_MPCBB_ConfigMemAttributes(), HAL_GTZC_MPCBB_GetConfigMem(), HAL_GTZC_MPCBB_GetConfigMemAttributes(), HAL_GTZC_MPCBB_GetLock(), HAL_GTZC_MPCBB_GetLockConfig() (+19 more)

### Community 23 - "HAL ICACHE Driver"
Cohesion: 0.16
Nodes (18): HAL_StatusTypeDef, __weak, HAL_ICACHE_ConfigAssociativityMode(), HAL_ICACHE_DeInit(), HAL_ICACHE_Disable(), HAL_ICACHE_DisableRemapRegion(), HAL_ICACHE_Enable(), HAL_ICACHE_EnableRemapRegion() (+10 more)

### Community 24 - "HAL RCC Clock Config"
Cohesion: 0.15
Nodes (18): HAL_StatusTypeDef, __weak, HAL_RCC_ClockConfig(), HAL_RCC_CSSCallback(), HAL_RCC_DeInit(), HAL_RCC_GetClockConfig(), HAL_RCC_GetConfigAttributes(), HAL_RCC_GetHCLKFreq() (+10 more)

### Community 25 - "HAL GPIO Driver"
Cohesion: 0.17
Nodes (20): GPIO_TypeDef, HAL_StatusTypeDef, __weak, HAL_GPIO_ConfigPinAttributes(), HAL_GPIO_DeInit(), HAL_GPIO_DisableHighSPeedLowVoltage(), HAL_GPIO_EnableHighSPeedLowVoltage(), HAL_GPIO_EXTI_Falling_Callback() (+12 more)

### Community 26 - "HAL Power Driver"
Cohesion: 0.10
Nodes (7): HAL_StatusTypeDef, __weak, HAL_PWR_ConfigPVD(), HAL_PWR_GetConfigAttributes(), HAL_PWR_PVD_IRQHandler(), HAL_PWR_PVDCallback(), PWR_PVDTypeDef

### Community 27 - "HAL RCC CRS Extended"
Cohesion: 0.10
Nodes (9): HAL_RCCEx_CRSConfig(), HAL_RCCEx_CRSGetSynchronizationInfo(), HAL_RCCEx_CRSWaitSynchronization(), HAL_RCCEx_LSECSS_Callback(), HAL_RCCEx_LSECSS_IRQHandler(), HAL_RCCEx_MSIPLLUNLCK_Callback(), HAL_RCCEx_MSIPLLUNLCK_IRQHandler(), RCC_CRSInitTypeDef (+1 more)

### Community 30 - "Application Init & MSP"
Cohesion: 0.20
Nodes (14): Error_Handler(), main(), MX_GPDMA1_Init(), MX_GPIO_Init(), MX_ICACHE_Init(), MX_RTC_Init(), MX_SPI1_Init(), SystemClock_Config() (+6 more)

### Community 31 - "HAL EXTI Driver"
Cohesion: 0.20
Nodes (14): HAL_StatusTypeDef, HAL_EXTI_ClearConfigLine(), HAL_EXTI_ClearPending(), HAL_EXTI_GenerateSWI(), HAL_EXTI_GetConfigLine(), HAL_EXTI_GetConfigLineAttributes(), HAL_EXTI_GetHandle(), HAL_EXTI_GetPending() (+6 more)

### Community 32 - "HAL RCC PLL Extended"
Cohesion: 0.17
Nodes (16): HAL_StatusTypeDef, HAL_RCCEx_DisableMSIPLLFastStartup(), HAL_RCCEx_DisablePLL2(), HAL_RCCEx_DisablePLL3(), HAL_RCCEx_EnableMSIPLLFastStartup(), HAL_RCCEx_EnableMSIPLLModeSelection(), HAL_RCCEx_EnablePLL2(), HAL_RCCEx_EnablePLL3() (+8 more)

### Community 33 - "Interrupt Handlers"
Cohesion: 0.14
Nodes (6): GPDMA1_Channel0_IRQHandler(), GPDMA1_Channel1_IRQHandler(), SPI1_IRQHandler(), SysTick_Handler(), HAL_DMA_IRQHandler(), HAL_IncTick()

### Community 34 - "HAL Voltage Scaling"
Cohesion: 0.15
Nodes (13): HAL_StatusTypeDef, HAL_PWREx_ConfigPVM(), HAL_PWREx_ConfigSupply(), HAL_PWREx_ControlVoltageScaling(), HAL_PWREx_DisableGPIOPullDown(), HAL_PWREx_DisableGPIOPullUp(), HAL_PWREx_EnableBkupRAMRetention(), HAL_PWREx_EnableGPIOPullDown() (+5 more)

### Community 35 - "HAL Init & Systick"
Cohesion: 0.18
Nodes (12): __weak, HAL_NVIC_SetPriority(), HAL_NVIC_SetPriorityGrouping(), HAL_SYSTICK_CLKSourceConfig(), HAL_SYSTICK_Config(), HAL_SYSTICK_GetCLKSourceConfig(), HAL_Delay(), HAL_Init() (+4 more)

### Community 36 - "HAL I2C Extended"
Cohesion: 0.39
Nodes (11): HAL_StatusTypeDef, I2C_HandleTypeDef, HAL_I2CEx_ClearConfigAutonomousMode(), HAL_I2CEx_ConfigAnalogFilter(), HAL_I2CEx_ConfigDigitalFilter(), HAL_I2CEx_ConfigFastModePlus(), HAL_I2CEx_DisableWakeUp(), HAL_I2CEx_EnableWakeUp() (+3 more)

### Community 37 - "HAL SPI Autonomous Mode"
Cohesion: 0.44
Nodes (9): HAL_StatusTypeDef, SPI_HandleTypeDef, HAL_SPIEx_ClearConfigAutonomousMode(), HAL_SPIEx_ConfigureUnderrun(), HAL_SPIEx_EnableLockConfiguration(), HAL_SPIEx_FlushRxFifo(), HAL_SPIEx_GetConfigAutonomousMode(), HAL_SPIEx_SetConfigAutonomousMode() (+1 more)

### Community 38 - "Driver Init Entry Points"
Cohesion: 0.22
Nodes (9): w25qxx_bool_t, w25qxx_interface_t, w25qxx_type_t, w25qxx_advance_init(), w25qxx_bool_t, w25qxx_interface_t, w25qxx_type_t, w25qxx_basic_init() (+1 more)

### Community 39 - "W25QXX SPI Interface Layer"
Cohesion: 0.25
Nodes (3): SPI_HandleTypeDef, HAL_SPI_ErrorCallback(), HAL_SPI_TxRxCpltCallback()

### Community 41 - "LL Device Utilities"
Cohesion: 0.43
Nodes (7): __STATIC_INLINE, LL_GetFlashSize(), LL_GetPackageType(), LL_GetUID_Word0(), LL_GetUID_Word1(), LL_GetUID_Word2(), LL_InitTick()

### Community 42 - "HAL Voltage Monitor Callbacks"
Cohesion: 0.36
Nodes (8): __weak, HAL_PWREx_AVM1Callback(), HAL_PWREx_AVM2Callback(), HAL_PWREx_IO2VMCallback(), HAL_PWREx_PVD_PVM_IRQHandler(), HAL_PWREx_S3WU_IRQHandler(), HAL_PWREx_S3WUCallback(), HAL_PWREx_UVMCallback()

### Community 43 - "HAL PLL Frequency Query"
Cohesion: 0.29
Nodes (7): HAL_RCCEx_GetPeriphCLKFreq(), HAL_RCCEx_GetPLL1ClockFreq(), HAL_RCCEx_GetPLL2ClockFreq(), HAL_RCCEx_GetPLL3ClockFreq(), PLL1_ClocksTypeDef, PLL2_ClocksTypeDef, PLL3_ClocksTypeDef

### Community 44 - "HAL CRS Callbacks"
Cohesion: 0.53
Nodes (6): __weak, HAL_RCCEx_CRS_ErrorCallback(), HAL_RCCEx_CRS_ExpectedSyncCallback(), HAL_RCCEx_CRS_IRQHandler(), HAL_RCCEx_CRS_SyncOkCallback(), HAL_RCCEx_CRS_SyncWarnCallback()

### Community 45 - "HAL Backup Domain Access"
Cohesion: 0.67
Nodes (4): HAL_PWR_DisableBkUpAccess(), HAL_PWR_EnableBkUpAccess(), HAL_RCCEx_DisableLSCO(), HAL_RCCEx_EnableLSCO()

## Knowledge Gaps
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `HAL_GetTick()` connect `HAL DMA Driver` to `HAL RCC PLL Extended`, `HAL Init & Systick`, `HAL SPI Driver`, `HAL DMA Linked Lists`, `HAL Internal Flash`, `HAL Core & Tick`, `HAL ICACHE Driver`, `HAL RCC Clock Config`, `HAL RCC CRS Extended`?**
  _High betweenness centrality (0.047) - this node is a cross-community bridge._
- **Why does `HAL_RCC_OscConfig()` connect `HAL RCC Clock Config` to `HAL Init & Systick`, `HAL DMA Driver`?**
  _High betweenness centrality (0.011) - this node is a cross-community bridge._
- **Why does `RCC_SetFlashLatencyFromMSIRange()` connect `HAL RCC Clock Config` to `HAL Voltage Scaling`?**
  _High betweenness centrality (0.009) - this node is a cross-community bridge._
- **Should `LL RCC Clock Registers` be split into smaller, more focused modules?**
  _Cohesion score 0.011975831252939686 - nodes in this community are weakly interconnected._
- **Should `LL GPDMA Register Layer` be split into smaller, more focused modules?**
  _Cohesion score 0.034876435781413154 - nodes in this community are weakly interconnected._
- **Should `LL SPI Register Layer` be split into smaller, more focused modules?**
  _Cohesion score 0.0397242880464357 - nodes in this community are weakly interconnected._
- **Should `HAL DMA Driver` be split into smaller, more focused modules?**
  _Cohesion score 0.06834733893557422 - nodes in this community are weakly interconnected._