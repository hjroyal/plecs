/*
   Copyright (c) 2014-2016 by Plexim GmbH
   All rights reserved.

   A free license is granted to anyone to use this software for any legal
   non safety-critical purpose, including commercial applications, provided
   that:
   1) IT IS NOT USED TO DIRECTLY OR INDIRECTLY COMPETE WITH PLEXIM, and
   2) THIS COPYRIGHT NOTICE IS PRESERVED in its entirety.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

#include "inc/hw_memmap.h"
#include "plx_ain.h"

#pragma diag_suppress 112  // PLX_ASSERT(0) in switch statements

/* Note:
 * No power-up delay implemented.
 * Assuming system-level delay will allow ADC to settle
 */

static uint32_t PLX_AIN_SysClkHz;

extern void AdcSetMode(Uint16 adc, Uint16 resolution, Uint16 signalmode);

void PLX_AIN_sinit(uint32_t aSysClkHz) {
  PLX_AIN_SysClkHz = aSysClkHz;
}

void PLX_AIN_setDefaultAdcParams(PLX_AIN_AdcParams_t *aParams,
                                 bool useExternalVref, float aVref,
                                 bool sigmodeDifferential) {
  aParams->ADCCTL1.bit.INTPULSEPOS = 1;  // set pulse positions to late
  // max allowable ADC clock is 50 MHz (min 5 MHz)
  if (PLX_AIN_SysClkHz <= 50000000L) {
    aParams->ADCCTL2.bit.PRESCALE = 0;  // set ADCCLK divider to /1.0
  } else if (PLX_AIN_SysClkHz <= 75000000L) {
    aParams->ADCCTL2.bit.PRESCALE = 1;  // set ADCCLK divider to /1.5
  } else if (PLX_AIN_SysClkHz <= 100000000L) {
    aParams->ADCCTL2.bit.PRESCALE = 2;  // set ADCCLK divider to /2.0
  } else if (PLX_AIN_SysClkHz <= 125000000L) {
    aParams->ADCCTL2.bit.PRESCALE = 3;  // set ADCCLK divider to /2.5
  } else if (PLX_AIN_SysClkHz <= 150000000L) {
    aParams->ADCCTL2.bit.PRESCALE = 4;  // set ADCCLK divider to /3.0
  } else if (PLX_AIN_SysClkHz <= 175000000L) {
    aParams->ADCCTL2.bit.PRESCALE = 5;  // set ADCCLK divider to /3.5
  } else {
    aParams->ADCCTL2.bit.PRESCALE = 6;  // set ADCCLK divider to /4.0
  }
  aParams->ADCSOCPRICTL.bit.SOCPRIORITY = 0;  // round-robin
                                              // no high priority channels

  aParams->useExternalVref = useExternalVref;

#ifndef HAS_INTERNAL_VREF
  PLX_ASSERT(useExternalVref);  // internal reference not supported
                                // for this target
#endif

  aParams->vref = aVref;
  aParams->sigmodeDifferential = sigmodeDifferential;
}

PLX_AIN_Handle_t PLX_AIN_init(void *aMemory, const size_t aNumBytes) {
  PLX_AIN_Handle_t handle;

  if (aNumBytes < sizeof(PLX_AIN_Obj_t)) {
    return ((PLX_AIN_Handle_t)NULL);
  }

  // set handle
  handle = (PLX_AIN_Handle_t)aMemory;

  return handle;
}

/* Keeping with type 5 convention, all VREFs should be configured the same
   if internal referencing is supported (right now just F28P65x chip).
*/
static void SetAllVrefs(bool useExternalVref, float aVref) {
#ifdef ADCA_BASE
  SetVrefWrapper(ADC_ADCA, useExternalVref, aVref);
#endif
#ifdef ADCB_BASE
  SetVrefWrapper(ADC_ADCB, useExternalVref, aVref);
#endif
#ifdef ADCC_BASE
  SetVrefWrapper(ADC_ADCC, useExternalVref, aVref);
#endif
}

// configure ADC Unit
void PLX_AIN_configure(PLX_AIN_Handle_t aHandle, PLX_AIN_Unit_t aUnit,
                       const PLX_AIN_AdcParams_t *aParams) {
  PLX_AIN_Obj_t *obj = (PLX_AIN_Obj_t *)aHandle;

  uint16_t Resolution;
  uint16_t SignalMode;
  float vref = aParams->vref;

  if (aParams->sigmodeDifferential) {
    Resolution = ADC_RESOLUTION_16BIT;
    SignalMode = ADC_SIGNALMODE_DIFFERENTIAL;
    /* (AINp-AINn)=VrefHi*(2*ADCxResult/2^16 - 1) per table 11-5 in tech ref*/
    obj->voltsPerAdcDiffBit = 2.0 * vref / 65536.0f;
    obj->voltsOffsetAdcDiff = -1.0 * vref;
  } else {
    Resolution = ADC_RESOLUTION_12BIT;
    SignalMode = ADC_SIGNALMODE_SINGLE;
    obj->voltsPerAdcBit = vref / 4096.0f;
  }
  obj->sigmodeDifferential = aParams->sigmodeDifferential;

  EALLOW;
  switch (aUnit) {
    default:
      PLX_ASSERT(0);
      break;
#ifdef ADCA_BASE
    case PLX_AIN_ADC_A:
      obj->adc = &AdcaRegs;
      obj->results = (uint16_t *)&AdcaResultRegs;
      CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
      AdcSetMode(ADC_ADCA, Resolution, SignalMode);
      break;
#endif
#ifdef ADCB_BASE
    case PLX_AIN_ADC_B:
      obj->adc = &AdcbRegs;
      obj->results = (uint16_t *)&AdcbResultRegs;
      CpuSysRegs.PCLKCR13.bit.ADC_B = 1;
      AdcSetMode(ADC_ADCB, Resolution, SignalMode);
      break;
#endif
#ifdef ADCC_BASE
    case PLX_AIN_ADC_C:
      obj->adc = &AdccRegs;
      obj->results = (uint16_t *)&AdccResultRegs;
      CpuSysRegs.PCLKCR13.bit.ADC_C = 1;
      AdcSetMode(ADC_ADCC, Resolution, SignalMode);
      break;
#endif
#ifdef ADCD_BASE
    case PLX_AIN_ADC_D:
      obj->adc = &AdcdRegs;
      obj->results = (uint16_t *)&AdcdResultRegs;
      CpuSysRegs.PCLKCR13.bit.ADC_D = 1;
      AdcSetMode(ADC_ADCD, Resolution, SignalMode);
      break;
#endif
  }

  EDIS;

#ifdef HAS_INTERNAL_VREF
  // For chips that support internal referencing, all ADC references should
  // be configured the same (similar to type 5 approach, allows reasonable
  // support for smaller pin count packages). Only applies to F28P65x currently
  SetAllVrefs(aParams->useExternalVref, aParams->vref);
#endif

  obj->socCtrl = (union ADCSOC0CTL_REG *)&obj->adc->ADCSOC0CTL;

  EALLOW;
  obj->adc->ADCCTL1.bit.ADCPWDNZ = 1;  // power up the ADC
  obj->adc->ADCCTL1.bit.INTPULSEPOS = aParams->ADCCTL1.bit.INTPULSEPOS;
  obj->adc->ADCCTL2.bit.PRESCALE = aParams->ADCCTL2.bit.PRESCALE;
  obj->adc->ADCSOCPRICTL.bit.SOCPRIORITY =
      aParams->ADCSOCPRICTL.bit.SOCPRIORITY;
  EDIS;
  PLX_AIN_resetChannelSetup(aHandle);
}

void PLX_AIN_setDefaultChannelParams(PLX_AIN_ChannelParams_t *aParams) {
  aParams->ADCSOCxCTL.bit.TRIGSEL = 1;  // trigger on CpuTimer0
  aParams->ADCSOCxCTL.bit.ACQPS = 14;   // safe default, overwritten later
                                        // based on user configuration
  aParams->scale = 1.0;
  aParams->offset = 0.0;
}

void PLX_AIN_resetChannelSetup(PLX_AIN_Handle_t aHandle) {
  PLX_AIN_Obj_t *obj = (PLX_AIN_Obj_t *)aHandle;

  EALLOW;
  for (int i = 0; i < PLX_AIN_NUM_CHANNELS; i++) {
    obj->socCtrl[i].bit.TRIGSEL = 0;
  }
  EDIS;
}

void PLX_AIN_setupChannel(PLX_AIN_Handle_t aHandle, uint16_t aChannel,
                          uint16_t aSource,
                          const PLX_AIN_ChannelParams_t *aParams) {
  PLX_AIN_Obj_t *obj = (PLX_AIN_Obj_t *)aHandle;

  PLX_ASSERT(aChannel < PLX_AIN_NUM_CHANNELS);
  PLX_ASSERT(aParams->ADCSOCxCTL.bit.TRIGSEL <= 0x1F);

  EALLOW;
  obj->socCtrl[aChannel].bit.CHSEL = aSource;
  obj->socCtrl[aChannel].bit.TRIGSEL = aParams->ADCSOCxCTL.bit.TRIGSEL;
  obj->socCtrl[aChannel].bit.ACQPS = aParams->ADCSOCxCTL.bit.ACQPS;
  EDIS;

  if (obj->sigmodeDifferential) {
    obj->scale[aChannel] = aParams->scale * obj->voltsPerAdcDiffBit;
    obj->offset[aChannel] = aParams->offset
                            + aParams->scale * obj->voltsOffsetAdcDiff;
  } else {
    obj->scale[aChannel] = aParams->scale * obj->voltsPerAdcBit;
    obj->offset[aChannel] = aParams->offset;
  }
}
