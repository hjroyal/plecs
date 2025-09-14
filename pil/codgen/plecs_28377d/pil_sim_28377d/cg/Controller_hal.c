/*
 * Hardware configuration file for: TI2837x
 * Generated with                 : PLECS 4.9.2
 * Generated on                   : Sun Jun  1 02:49:16 2025
 */

/* HAL Includes */
#include "Controller.h"
#include "plx_hal.h"
#include "plx_dispatcher.h"
#include "pil.h"
#include "plx_sci.h"
#include "string.h"
#include "pin_map.h"
#include "gpio.h"
#include "xbar.h"
#include "asysctl.h"
#include "sysctl.h"
#include "cmpss.h"
#include "plx_ain.h"
#include "plx_power.h"
#include "plx_pwm.h"
#include "math.h"
#include "epwm.h"
#include "hrpwm.h"
#include "plx_dio.h"

/* HAL Declarations */
void DevInit(uint32_t aDeviceClkConf);
void InitFlashHz(Uint32 clkHz);
void DSP28x_usDelay(long LoopCount);

// Clock configuration
#define PLX_DEVICE_SETCLOCK_CFG  (SYSCTL_OSCSRC_OSC2 | SYSCTL_IMULT(19) |  \
                                  SYSCTL_FMULT_0 | SYSCTL_SYSDIV(1) |   \
                                  SYSCTL_PLL_ENABLE)

#define SYSCLK_HZ 190000000L
#define SYSCLK_WC_HI_HZ 196000000L
#define LSPCLK_HZ (SYSCLK_HZ / 4l)
#define PLX_DELAY_US(A)  DSP28x_usDelay( \
                                        ((((long double) A * \
                                           1000.0L) / \
                                          5.263158L) - 9.0L) / 5.0L)

PIL_Obj_t PilObj;
PIL_Handle_t PilHandle = 0;
// external mode helper symbols
PIL_CONFIG_DEF(uint32_t, ExtMode_targetFloat_Size,
               sizeof(Controller_FloatType));
PIL_CONFIG_DEF(uint32_t, ExtMode_targetPointer_Size,
               sizeof(Controller_FloatType*));
PIL_CONFIG_DEF(uint32_t, ExtMode_sampleTime_Ptr,
               (uint32_t)&Controller_sampleTime);
PIL_CONFIG_DEF(uint32_t, ExtMode_checksum_Ptr,
               (uint32_t)&Controller_checksum);
#if defined(Controller_NumTunableParameters) && \
   (Controller_NumTunableParameters > 0)
PIL_CONFIG_DEF(uint32_t, ExtMode_P_Ptr, (uint32_t)&Controller_P);
PIL_CONFIG_DEF(uint32_t, ExtMode_P_Size,
               (uint32_t)Controller_NumTunableParameters);
#endif
#if defined(Controller_NumExtModeSignals) && (Controller_NumExtModeSignals > \
                                              0)
PIL_CONFIG_DEF(uint32_t, ExtMode_ExtModeSignals_Ptr,
               (uint32_t)&Controller_ExtModeSignals[0]);
PIL_CONFIG_DEF(uint32_t, ExtMode_ExtModeSignals_Size,
               (uint32_t)Controller_NumExtModeSignals);
#endif

#define CODE_GUID {0x09, 0x3c, 0x97, 0x5e, 0xdb, 0x2d, 0xb8, 0x40};
PIL_CONST_DEF(unsigned char, Guid[], CODE_GUID);
PIL_CONST_DEF(unsigned char, CompiledDate[], "06/01/2025 02:49 AM");
PIL_CONST_DEF(unsigned char, CompiledBy[], "PLECS Coder");
PIL_CONST_DEF(uint16_t, FrameworkVersion, PIL_FRAMEWORK_VERSION);
PIL_CONST_DEF(char, FirmwareDescription[], "TIC2000 Project (CPU0)");
PIL_CONST_DEF(uint16_t, StationAddress, 0);
PIL_CONST_DEF(uint32_t, BaudRate, 115200);
static void SciPoll(PIL_Handle_t aHandle)
{
   PLXHAL_SCI_handleBreak(0);

   while(PLXHAL_SCI_rxIsReady(0))
   {
      // assuming that there will be a "break" when FIFO is empty
      PIL_SERIAL_IN(aHandle, (int16) PLXHAL_SCI_getChar(0));
   }
   int16_t ch;
   if(!PLXHAL_SCI_txIsBusy(0))
   {
      if(PIL_SERIAL_OUT(aHandle, &ch))
      {
         PLXHAL_SCI_putChar(0, ch);
      }
   }
}

#pragma DATA_SECTION(ScopeBuffer, "scope")
uint16_t ScopeBuffer[2020] /*__attribute__((aligned(16)))*/;
extern void PIL_setAndConfigScopeBuffer(PIL_Handle_t aPilHandle,
                                        uint16_t* aBufPtr, uint16_t aBufSize,
                                        uint16_t aMaxTraceWidthInWords);
extern const char * const Controller_checksum;

uint16_t ScopeFlagCpuRemote;
#pragma DATA_SECTION(ScopeFlagCpuRemote, "scopeflag_remote")
#pragma RETAIN(ScopeFlagCpuRemote)
uint16_t ScopeFlagCpuThis;
#pragma DATA_SECTION(ScopeFlagCpuThis, "scopeflag_local")
#pragma RETAIN(ScopeFlagCpuThis)
PIL_SYMBOL_DEF(ScopeFlagCpuRemote, 0, 1.0, "");
PIL_SYMBOL_DEF(ScopeFlagCpuThis, 0, 1.0, "");
extern void PIL_setAndConfigureScopeIndicator(PIL_Handle_t aPilHandle,
                                              uint16_t* aIndicatorPtr);

PLX_SCI_Obj_t SciObjs[1];
PLX_SCI_Handle_t SciHandles[1];

uint16_t PLXHAL_SCI_getChar(int16_t aChannel)
{
   return PLX_SCI_getChar(SciHandles[aChannel]);
}

void PLXHAL_SCI_putChar(int16_t aChannel, uint16_t data)
{
   PLX_SCI_putChar(SciHandles[aChannel], data);
}

void PLXHAL_SCI_putStr(int16_t aChannel, uint16_t *str)
{
   for (int i = 0; i < strlen((char *)str); i++)
   {
      PLXHAL_SCI_putChar(aChannel, str[i]);
   }
}

bool PLXHAL_SCI_rxIsReady(int16_t aChannel)
{
   return PLX_SCI_rxReady(SciHandles[aChannel]);
}

bool PLXHAL_SCI_txIsBusy(int16_t aChannel)
{
   return PLX_SCI_txIsBusy(SciHandles[aChannel]);
}

void PLXHAL_SCI_handleBreak(int16_t aChannel)
{
   if(PLX_SCI_breakOccurred(SciHandles[aChannel]))
   {
      PLX_SCI_reset(SciHandles[aChannel]);
   }
}

uint32_t PLXHAL_DISPR_getTimeStamp0()
{
   return DISPR_getTimeStamp0();
}

uint32_t PLXHAL_DISPR_getTimeStamp1()
{
   return DISPR_getTimeStamp1();
}

uint32_t PLXHAL_DISPR_getTimeStamp2()
{
   return DISPR_getTimeStamp2();
}

uint32_t PLXHAL_DISPR_getTimeStamp3()
{
   return DISPR_getTimeStamp3();
}

uint32_t PLXHAL_DISPR_getTimeStampB()
{
   return DISPR_getTimeStampB();
}

uint32_t PLXHAL_DISPR_getTimeStampD()
{
   return DISPR_getTimeStampD();
}

uint32_t PLXHAL_DISPR_getTimeStampP()
{
   return DISPR_getTimeStampP();
}

float PLXHAL_DISPR_getTask0LoadInPercent()
{
   return DISPR_getTask0LoadInPercent();
}

PLX_AIN_Handle_t AdcHandles[2];
PLX_AIN_Obj_t AdcObj[2];

float PLXHAL_ADC_getIn(uint16_t aHandle, uint16_t aChannel)
{
   return PLX_AIN_getInF(AdcHandles[aHandle], aChannel);
}

void PLXHAL_PWR_setEnableRequest(bool aEnable)
{
   PLX_PWR_setEnableRequest(aEnable);
   PLX_PWR_runFsm();
}
bool PLXHAL_PWR_isEnabled()
{
   return PLX_PWR_isEnabled();
}
void PLXHAL_PWR_syncdPwmEnable()
{
   PLX_PWR_syncdSwitchingEnable();
}
extern PLX_PWM_Handle_t EpwmHandles[];
// PWM Enable Functions
void PLXHAL_PWM_setToPassive(uint16_t aChannel)
{
   PLX_PWM_setOutToPassive(EpwmHandles[aChannel]);
}

void PLXHAL_PWM_setToOperational(uint16_t aChannel)
{
   PLX_PWM_setOutToOperational(EpwmHandles[aChannel]);
}

void PLXHAL_PWM_prepareSetOutToXTransition(uint16_t aHandle)
{
   PLX_PWM_prepareSetOutToXTransition(EpwmHandles[aHandle]);
}

// PWM Set Duty Cycle Function
void PLXHAL_PWM_setDuty(uint16_t aHandle, float aDuty)
{
   PLX_PWM_setPwmDuty(EpwmHandles[aHandle], aDuty);
}

// PWM Set Dead Time Functions
void PLXHAL_PWM_setRisingDelay(uint16_t aChannel, float delay)
{
   uint16_t counts = (uint16_t)(fminf(65535.0f, delay * 4.750000e+07f));
   PLX_PWM_setDeadTimeCountsRising(EpwmHandles[aChannel], counts);
}

void PLXHAL_PWM_setFallingDelay(uint16_t aChannel, float delay)
{
   uint16_t counts = (uint16_t)(fminf(65535.0f, delay * 4.750000e+07f));
   PLX_PWM_setDeadTimeCountsFalling(EpwmHandles[aChannel], counts);
}

// PWM Set Sequence Functions
void PLXHAL_PWM_setSequence(uint16_t aChannel, uint16_t aSequence)
{
   PLX_PWM_setSequence(EpwmHandles[aChannel], aSequence);
}

void PLXHAL_PWM_setSequenceAq(uint16_t aChannel, uint32_t aSequenceAq)
{
   PLX_PWM_setSequenceAq(EpwmHandles[aChannel], aSequenceAq);
}

void PLXHAL_PWM_setDutyAB(uint16_t aHandle, float aDutyA, float aDutyB)
{
   PLX_PWM_setPwmDuty(EpwmHandles[aHandle], aDutyA);
   PLX_PWM_setPwmDutyB(EpwmHandles[aHandle], aDutyB);
}

void PLXHAL_PWM_setOverflowSocDelay(uint16_t aHandle, float aPhase)
{
   PLX_PWM_setPwmDutyCGreaterThanZero(EpwmHandles[aHandle], 1.0f - aPhase);
}

void PLXHAL_PWM_setUnderflowSocDelay(uint16_t aHandle, float aPhase)
{
   PLX_PWM_setPwmDutyCSmallerThanFull(EpwmHandles[aHandle], aPhase);
}

PLX_PWM_Handle_t EpwmHandles[1];
PLX_PWM_Obj_t EpwmObj[1];
PLX_DIO_Handle_t DoutHandles[2];
PLX_DIO_Obj_t DoutObj[2];

void PLXHAL_DIO_set(uint16_t aHandle, bool aVal)
{
   PLX_DIO_set(DoutHandles[aHandle], aVal);
}

void PIL_setErrorMessage(PIL_Handle_t aPilHandle, const char* aMessage);
extern const char * Controller_errorStatus;

static void PanicCallback()
{
   PLX_PWR_emergencyOff();
}

extern PIL_Handle_t PilHandle;
DISPR_TaskObj_t TaskObj[2];
extern void Controller_step(int task_id);
extern void Controller_enableTasksInterrupt();
extern void Controller_syncTimers();
static void Tasks(bool aInit, void * const aParam)
{
   if(Controller_errorStatus)
   {
      PIL_setErrorMessage(PilHandle, Controller_errorStatus);
      return;
   }
   if(aInit)
   {
      Controller_enableTasksInterrupt();
   }
   else
   {
      Controller_step(*(int *)aParam);
   }
}


interrupt void Controller_baseTaskInterrupt(void)
{
   AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;              // clear ADCINT1 flag reinitialize for next SOC
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;  // acknowledge interrupt to PIE (All ADCS in group 1)
   IER |= M_INT1;

   DISPR_dispatch();
}


/* Interrupt Enable Code */
void Controller_enableTasksInterrupt(void)
{
   IER |= M_INT1;
}

/* Timer Synchronization Code */
void Controller_syncTimers(void)
{
   CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC = 1;  // start all the timers synced (global)

}

/* Background tasks */
void Controller_background(void)
{

}
/* HAL Initialization Code */
static bool HalInitialized = false;
void Controller_initHal()
{
   if(HalInitialized == true)
   {
      return;
   }
   HalInitialized = true;
   // Pre init code
   {
      uint32_t sysPllConfig = PLX_DEVICE_SETCLOCK_CFG;
      DevInit(sysPllConfig);
      SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);

      PLX_ASSERT(SysCtl_getClock(10000000) == SYSCLK_HZ);
      PLX_ASSERT(SysCtl_getLowSpeedClock(10000000) == LSPCLK_HZ);
   }
   InitFlashHz(SYSCLK_WC_HI_HZ);
   // set cpu timers to same clock as ePWM
   CpuTimer0Regs.TPR.all = 3;
   CpuTimer1Regs.TPR.all = 3;
   CpuTimer2Regs.TPR.all = 3;
   EALLOW;
   CpuSysRegs.PCLKCR0.bit.CPUTIMER0 = 1;
   CpuSysRegs.PCLKCR0.bit.CPUTIMER1 = 1;
   CpuSysRegs.PCLKCR0.bit.CPUTIMER2 = 1;
   CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;  // stop all the TB clocks on the local cpu
   CpuSysRegs.PCLKCR0.bit.GTBCLKSYNC = 0;  // stop all the TB clocks
   ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 1;
   EDIS;

   PilHandle = PIL_init(&PilObj, sizeof(PilObj));
   PIL_setGuid(PilHandle, PIL_GUID_PTR);
   PIL_setChecksum(PilHandle, Controller_checksum);
   PIL_setAndConfigScopeBuffer(PilHandle, (uint16_t *)&ScopeBuffer, 2020, 10);
   PIL_setAndConfigureScopeIndicator(PilHandle, &ScopeFlagCpuThis);
   PIL_setNodeAddress(PilHandle, PIL_D_StationAddress);

   PIL_setSerialComCallback(PilHandle, (PIL_CommCallbackPtr_t)SciPoll);
   {
      for(int i=0; i < 1; i++)
      {
         SciHandles[i] =
            PLX_SCI_init(&SciObjs[i],
                         sizeof(SciObjs[i]));
      }
   }

   PLX_SCI_configure(SciHandles[0],
                     PLX_SCI_SCI_A, 47500000);

   (void)PLX_SCI_setupPort(SciHandles[0], 115200);
   {
      // early system configuration
      PLX_DIO_sinit();
   }
   {
      SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
      CMPSS_enableModule(CMPSS2_BASE);
      CMPSS_configHighComparator(CMPSS2_BASE, CMPSS_INSRC_DAC);
      CMPSS_configDAC(CMPSS2_BASE, CMPSS_DACREF_VDDA | CMPSS_DACVAL_SYSCLK |
                      CMPSS_DACSRC_SHDW);
      CMPSS_setDACValueHigh(CMPSS2_BASE, 3270);             // 2.634146 V
      CMPSS_configFilterHigh(CMPSS2_BASE, 0, 10, 10);
      CMPSS_initFilterHigh(CMPSS2_BASE);

      CMPSS_configOutputsHigh(CMPSS2_BASE, CMPSS_TRIP_FILTER);

      // low comparator not used
   }

   {
      PLX_AIN_sinit(190000000);

      for (int i = 0; i < 2; i++)
      {
         AdcHandles[i] = PLX_AIN_init(&AdcObj[i], sizeof(AdcObj[i]));
      }
   }


   // configure ADC B
   {
      PLX_AIN_AdcParams_t params;
      PLX_AIN_setDefaultAdcParams(&params, 1, 3.000000, 0);
      PLX_AIN_configure(AdcHandles[1], (PLX_AIN_Unit_t)1, &params);
   }
   // configure SOC1 of ADC-B to measure ADCIN4
   {
      PLX_AIN_ChannelParams_t params;
      PLX_AIN_setDefaultChannelParams(&params);
      params.scale =  4.030303030e+00f;
      params.offset = 0.000000000e+00f;

      // set SOC trigger to PWM4
      params.ADCSOCxCTL.bit.TRIGSEL = 11;
      params.ADCSOCxCTL.bit.ACQPS = 14;

      PLX_AIN_setupChannel(AdcHandles[1], 0, 4, &params);
   }


   // configure ADC A
   {
      PLX_AIN_AdcParams_t params;
      PLX_AIN_setDefaultAdcParams(&params, 1, 3.000000, 0);
      PLX_AIN_configure(AdcHandles[0], (PLX_AIN_Unit_t)0, &params);
   }
   // configure SOC1 of ADC-A to measure ADCIN3
   {
      PLX_AIN_ChannelParams_t params;
      PLX_AIN_setDefaultChannelParams(&params);
      params.scale =  2.030303030e+00f;
      params.offset = 0.000000000e+00f;

      // set SOC trigger to PWM4
      params.ADCSOCxCTL.bit.TRIGSEL = 11;
      params.ADCSOCxCTL.bit.ACQPS = 14;

      PLX_AIN_setupChannel(AdcHandles[0], 0, 3, &params);
   }

   // configure SOC2 of ADC-A to measure ADCIN4
   {
      PLX_AIN_ChannelParams_t params;
      PLX_AIN_setDefaultChannelParams(&params);
      params.scale =  2.277777778e+00f;
      params.offset = 0.000000000e+00f;

      // set SOC trigger to PWM4
      params.ADCSOCxCTL.bit.TRIGSEL = 11;
      params.ADCSOCxCTL.bit.ACQPS = 14;

      PLX_AIN_setupChannel(AdcHandles[0], 1, 4, &params);
   }

   {
      PLX_PWR_sinit();
      PLX_DIO_sinit();
      static PLX_DIO_Obj_t doutObj;
      PLX_DIO_Handle_t doutHandle = PLX_DIO_init(&doutObj, sizeof(doutObj));
      PLX_DIO_OutputProperties_t props = {
         0
      };
      props.enableInvert = true;
      PLX_DIO_configureOut(doutHandle, 1, &props);
      PLX_PWR_configure(doutHandle, 66667);
   }
   PLX_PWM_sinit();
   for (int i = 0; i < 1; i++)
   {
      EpwmHandles[i] = PLX_PWM_init(&EpwmObj[i], sizeof(EpwmObj[i]));
   }

   // configure PWM4 at 199579.8 Hz in triangle mode
   // soc='p'   desired frequency was 200000 Hz
   {
      PLX_PWM_Params_t params;
      PLX_PWM_setDefaultParams(&params);
      params.outMode = PLX_PWM_OUTPUT_MODE_DUAL;
      params.reg.TBPRD = 119;
      params.reg.TBCTL.bit.CTRMODE = 2;

      params.reg.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
      // active state is high
      params.reg.DBCTL.bit.POLSEL = 2; // DB_ACTV_HIC
      // enable deadtime insertion
      params.reg.DBCTL.bit.OUT_MODE = 3; // DB_FULL_ENABLE
      params.reg.DBCTL.bit.IN_MODE = 0; // DBA_ALL
      // TZ settings
      params.reg.TZSEL.bit.CBC1 = 0;
      params.reg.TZSEL.bit.OSHT1 = 0;
      params.reg.TZSEL.bit.CBC2 = 0;
      params.reg.TZSEL.bit.OSHT2 = 0;
      params.reg.TZSEL.bit.CBC3 = 0;
      params.reg.TZSEL.bit.OSHT3 = 0;
      // force low when tripped
      params.reg.TZCTL.bit.TZA = 2;
      params.reg.TZCTL.bit.TZB = 2;
      PLX_PWM_configure(EpwmHandles[0], 4, &params);
      // configure deadtime to 0.000000e+00/0.000000e+00 seconds (rising/falling)
      PLX_PWM_setDeadTimeCounts(EpwmHandles[0], 0, 0);

      // PWM sequence starting with active state
      PLX_PWM_setSequence(EpwmHandles[0], 1);

      EPwm4Regs.ETSEL.bit.SOCASEL = 2;
      EPwm4Regs.ETPS.bit.SOCPSSEL = 1;
      EPwm4Regs.ETSOCPS.bit.SOCAPRD2 = 3;
      EPwm4Regs.ETSEL.bit.SOCAEN = 1;

      EPWM_setTripZoneDigitalCompareEventCondition(EPWM4_BASE,
                                                   EPWM_TZ_DC_OUTPUT_A1,
                                                   EPWM_TZ_EVENT_DCXH_HIGH);
      EPWM_enableDigitalCompareTripCombinationInput(EPWM4_BASE,
                                                    EPWM_DC_COMBINATIONAL_TRIPIN4,
                                                    EPWM_DC_TYPE_DCAH);

      EPWM_setDigitalCompareEventSource(EPWM4_BASE,
                                        EPWM_DC_MODULE_A,
                                        EPWM_DC_EVENT_1,
                                        EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL);

      EPWM_enableTripZoneSignals(EPWM4_BASE, EPWM_TZ_SIGNAL_DCAEVT1);

   }
   PLX_DIO_sinit();
   for (int i = 0; i < 2; i++)
   {
      DoutHandles[i] = PLX_DIO_init(&DoutObj[i], sizeof(DoutObj[i]));
   }

   {
      PLX_DIO_OutputProperties_t props = {
         0
      };

      props.enableInvert = false;
      PLX_DIO_configureOut(DoutHandles[0], 10,  &props);
   }

   {
      PLX_DIO_OutputProperties_t props = {
         0
      };

      props.enableInvert = false;
      PLX_DIO_configureOut(DoutHandles[1], 0,  &props);
   }

   DISPR_sinit();
   DISPR_configure((uint32_t)(712), PilHandle, &TaskObj[0],
                   sizeof(TaskObj)/sizeof(DISPR_TaskObj_t));
   DISPR_registerIdleTask(&Controller_background);
   DISPR_registerSyncCallback(&Controller_syncTimers);
   DISPR_setPowerupDelay(67);
   DISPR_registerPanicCallback(&PanicCallback);
   {
      static int taskId = 0;
      // Task 0 at 6.671348e+04 Hz
      DISPR_registerTask(0, &Tasks, 712L, (void *)&taskId);
   }
   {
      static int taskId = 1;
      // Task 1 at 2.001425e+00 Hz
      DISPR_registerTask(1, &Tasks, 23733096L, (void *)&taskId);
   }
   EALLOW;
   AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0;  // disable ADCINT1 Continuous mode
   AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;  // setup EOC1 to trigger ADCINT1
   AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;  // enable ADCINT1
   AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;  // ADCINT1 trips after AdcResults latch
   EDIS;
   EALLOW;
   *(PINT *)((uint32_t)(&PieVectTable.ADCA1_INT) + ((uint32_t)0)*
             sizeof(PINT *)) = &Controller_baseTaskInterrupt;
   PieCtrlRegs.PIEIER1.all |= 1;
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   EDIS;


   // Post init code (for modules that depend on other modules)

   {
      // late system configuration
      GPIO_setPinConfig(GPIO_6_EPWM4A);
      GPIO_setPinConfig(GPIO_7_EPWM4B);
      XBAR_setEPWMMuxConfig(XBAR_TRIP4, XBAR_EPWM_MUX02_CMPSS2_CTRIPH);
      XBAR_enableEPWMMux(XBAR_TRIP4, XBAR_MUX02);
      GPIO_setPadConfig(1, GPIO_PIN_TYPE_STD);
      GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);
      GPIO_setPadConfig(10, GPIO_PIN_TYPE_STD);
      GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);
      GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);
      GPIO_setDirectionMode(0, GPIO_DIR_MODE_OUT);
      GPIO_setPinConfig(GPIO_64_SCIRXDA);
      GPIO_setPinConfig(GPIO_65_SCITXDA);
   }
   PLX_PWR_registerPwmChannel(EpwmHandles[0]);
}
