/*
   Copyright (c) 2014-2024 by Plexim GmbH
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

#include "plx_pwm.h"

#pragma diag_suppress 112  // PLX_ASSERT(0) in switch statements

void PLX_PWM_sinit() {
  EALLOW;
#ifdef EPWM1_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
  EPwm1Regs.ETPS.bit.SOCPSSEL = 1;  // // enable larger period counter

  // relying on power-up defaults here
  // EPwm1Regs.TBCTL.bit.CLKDIV = 0; // = /1 (default)
  // EPwm2Regs.TBCTL.bit.CLKDIV = 0;
#endif
#ifdef EPWM2_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM2 = 1;
  EPwm2Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM3_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM3 = 1;
  EPwm3Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM4_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM4 = 1;
  EPwm4Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM5_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM5 = 1;
  EPwm5Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM6_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM6 = 1;
  EPwm6Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM7_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM7 = 1;
  EPwm7Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM8_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM8 = 1;
  EPwm8Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM9_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM9 = 1;
  EPwm9Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM10_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM10 = 1;
  EPwm10Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM11_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM11 = 1;
  EPwm11Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM12_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM12 = 1;
  EPwm12Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM13_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM13 = 1;
  EPwm13Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM14_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM14 = 1;
  EPwm14Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM15_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM15 = 1;
  EPwm15Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM16_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM16 = 1;
  EPwm16Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM17_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM17 = 1;
  EPwm17Regs.ETPS.bit.SOCPSSEL = 1;
#endif
#ifdef EPWM18_BASE
  CpuSysRegs.PCLKCR2.bit.EPWM18 = 1;
  EPwm18Regs.ETPS.bit.SOCPSSEL = 1;
#endif
  EDIS;
}

void PLX_PWM_getRegisterBase(PLX_PWM_Unit_t aPwmChannel,
                             volatile struct EPWM_REGS **aReg) {
  switch (aPwmChannel) {
#ifdef EPWM1_BASE
    case PLX_PWM_EPWM1:
      *aReg = &EPwm1Regs;
      break;
#endif
#ifdef EPWM2_BASE
    case PLX_PWM_EPWM2:
      *aReg = &EPwm2Regs;
      break;
#endif
#ifdef EPWM3_BASE
    case PLX_PWM_EPWM3:
      *aReg = &EPwm3Regs;
      break;
#endif
#ifdef EPWM4_BASE
    case PLX_PWM_EPWM4:
      *aReg = &EPwm4Regs;
      break;
#endif
#ifdef EPWM5_BASE
    case PLX_PWM_EPWM5:
      *aReg = &EPwm5Regs;
      break;
#endif
#ifdef EPWM6_BASE
    case PLX_PWM_EPWM6:
      *aReg = &EPwm6Regs;
      break;
#endif
#ifdef EPWM7_BASE
    case PLX_PWM_EPWM7:
      *aReg = &EPwm7Regs;
      break;
#endif
#ifdef EPWM8_BASE
    case PLX_PWM_EPWM8:
      *aReg = &EPwm8Regs;
      break;
#endif
#ifdef EPWM9_BASE
    case PLX_PWM_EPWM9:
      *aReg = &EPwm9Regs;
      break;
#endif
#ifdef EPWM10_BASE
    case PLX_PWM_EPWM10:
      *aReg = &EPwm10Regs;
      break;
#endif
#ifdef EPWM11_BASE
    case PLX_PWM_EPWM11:
      *aReg = &EPwm11Regs;
      break;
#endif
#ifdef EPWM12_BASE
    case PLX_PWM_EPWM12:
      *aReg = &EPwm12Regs;
      break;
#endif
#ifdef EPWM13_BASE
    case PLX_PWM_EPWM13:
      *aReg = &EPwm13Regs;
      break;
#endif
#ifdef EPWM14_BASE
    case PLX_PWM_EPWM14:
      *aReg = &EPwm14Regs;
      break;
#endif
#ifdef EPWM15_BASE
    case PLX_PWM_EPWM15:
      *aReg = &EPwm15Regs;
      break;
#endif
#ifdef EPWM16_BASE
    case PLX_PWM_EPWM16:
      *aReg = &EPwm16Regs;
      break;
#endif
#ifdef EPWM17_BASE
    case PLX_PWM_EPWM17:
      *aReg = &EPwm17Regs;
      break;
#endif
#ifdef EPWM18_BASE
    case PLX_PWM_EPWM18:
      *aReg = &EPwm18Regs;
      break;
#endif
    default:
      PLX_ASSERT(0);
      break;
  }
}

void PLX_PWM_setDefaultParams(PLX_PWM_Params_t *aParams) {
  aParams->outMode = PLX_PWM_OUTPUT_MODE_DUAL;

  aParams->reg.TBPRD = 0;
  aParams->reg.DBFED.all = 0;
  aParams->reg.DBRED.all = 0;
  aParams->reg.TBPHS.bit.TBPHS = 0;  // set Phase register to zero
  aParams->reg.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // symmetrical mode
  aParams->reg.TBCTL.bit.PHSEN = TB_DISABLE;         // master module
  aParams->reg.TBCTL.bit.PRDLD = TB_SHADOW;
  aParams->reg.TBCTL.bit.HSPCLKDIV = TB_DIV1;
  aParams->reg.TBCTL.bit.CLKDIV = TB_DIV1;
  aParams->reg.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
  aParams->reg.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
  aParams->reg.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;  // load on CTR = Zero
                                                        // and Prd
  aParams->reg.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;  // load on CTR = Zero
                                                        // and Prd
  aParams->reg.AQCTLA.bit.CAU = AQ_SET;
  aParams->reg.AQCTLA.bit.CAD = AQ_CLEAR;
  aParams->reg.AQCTLA.bit.PRD = AQ_NO_ACTION;
  aParams->reg.AQCTLA.bit.ZRO = AQ_NO_ACTION;
  aParams->reg.DBCTL.bit.IN_MODE = DBA_ALL;
  aParams->reg.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // enable Dead-band module
  aParams->reg.DBCTL.bit.POLSEL = DB_ACTV_HIC;       // active Hi complementary
  aParams->reg.TZCTL.bit.TZA = TZ_NO_CHANGE;
  aParams->reg.TZCTL.bit.TZB = TZ_NO_CHANGE;
  aParams->reg.ETSEL.bit.INTSEL = ET_CTR_ZERO;

  aParams->reg.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
  aParams->reg.ETSEL.bit.SOCAEN = 0;
  aParams->reg.ETSOCPS.bit.SOCAPRD2 = 1;
}

PLX_PWM_Handle_t PLX_PWM_init(void *aMemory, const size_t aNumBytes) {
  PLX_PWM_Handle_t handle;

  if (aNumBytes < sizeof(PLX_PWM_Obj_t)) {
    return ((PLX_PWM_Handle_t)NULL);
  }

  // set handle
  handle = (PLX_PWM_Handle_t)aMemory;

  return handle;
}

void PLX_PWM_configure(PLX_PWM_Handle_t aHandle, uint16_t aModulator,
                       const PLX_PWM_Params_t *aParams) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  PLX_PWM_getRegisterBase((PLX_PWM_Unit_t)aModulator, &obj->pwm);

  EALLOW;
  obj->pwm->TBPRD = aParams->reg.TBPRD;
  obj->nomTBPRD = aParams->reg.TBPRD;
  obj->pwm->DBFED.all = aParams->reg.DBFED.all;
  obj->pwm->DBRED.all = aParams->reg.DBRED.all;
  obj->pwm->TBPHS.bit.TBPHS = aParams->reg.TBPHS.bit.TBPHS;
  obj->pwm->TBCTL.bit.CTRMODE = aParams->reg.TBCTL.bit.CTRMODE;
  obj->pwm->TBCTL.bit.PHSEN = aParams->reg.TBCTL.bit.PHSEN;
  obj->pwm->TBCTL.bit.PRDLD = aParams->reg.TBCTL.bit.PRDLD;
  obj->pwm->TBCTL.bit.HSPCLKDIV = aParams->reg.TBCTL.bit.HSPCLKDIV;
  obj->pwm->TBCTL.bit.CLKDIV = aParams->reg.TBCTL.bit.CLKDIV;
  obj->pwm->CMPCTL.bit.SHDWAMODE = aParams->reg.CMPCTL.bit.SHDWAMODE;
  obj->pwm->CMPCTL.bit.SHDWBMODE = aParams->reg.CMPCTL.bit.SHDWBMODE;
  obj->pwm->CMPCTL.bit.LOADAMODE = aParams->reg.CMPCTL.bit.LOADAMODE;
  obj->pwm->CMPCTL.bit.LOADBMODE = aParams->reg.CMPCTL.bit.LOADBMODE;

  // enable shadowing to allow synchronized PWM sequence modifications
  obj->pwm->AQCTL.bit.SHDWAQAMODE = 1;  // action control A shadowed
  obj->pwm->AQCTL.bit.LDAQAMODE =
      2;  // action control A loaded at zero and period

  obj->pwm->AQCTLA.bit.CAU = aParams->reg.AQCTLA.bit.CAU;
  obj->pwm->AQCTLA.bit.CAD = aParams->reg.AQCTLA.bit.CAD;
  obj->pwm->AQCTLA.bit.PRD = aParams->reg.AQCTLA.bit.PRD;
  obj->pwm->AQCTLA.bit.ZRO = aParams->reg.AQCTLA.bit.ZRO;

  // shadowing to allow synchronized disabling of leg
  obj->pwm->DBCTL2.bit.SHDWDBCTLMODE = 1;  // shadow [5:0] of DBCTL
  obj->pwm->DBCTL2.bit.LOADDBCTLMODE = 0;  // load at zero
  obj->pwm->AQSFRC.bit.RLDCSF = 0;         // load software force on zero

  obj->pwm->DBCTL.bit.IN_MODE = aParams->reg.DBCTL.bit.IN_MODE;
  obj->pwm->DBCTL.bit.OUT_MODE = aParams->reg.DBCTL.bit.OUT_MODE;
  obj->pwm->DBCTL.bit.POLSEL = aParams->reg.DBCTL.bit.POLSEL;

  obj->pwm->ETSEL.bit.INTSEL = aParams->reg.ETSEL.bit.INTSEL;
  obj->pwm->ETSEL.bit.SOCASEL = aParams->reg.ETSEL.bit.SOCASEL;
  obj->pwm->ETSEL.bit.SOCAEN = aParams->reg.ETSEL.bit.SOCAEN;
  obj->pwm->ETSOCPS.bit.SOCAPRD2 = aParams->reg.ETSOCPS.bit.SOCAPRD2;

  obj->pwm->TZCTL2.bit.ETZE = 0;  // use trip action from TZCTL
                                  // (legacy EPWM compatibility)

  obj->pwm->TZCTL.all = 0x0FFF;  // disable all trip actions
  obj->pwm->TZCTL.bit.TZA = aParams->reg.TZCTL.bit.TZA;
  obj->pwm->TZCTL.bit.TZB = aParams->reg.TZCTL.bit.TZB;

  obj->pwm->TZSEL.all = 0;  // disable all CBC and OSHT trip sources
  obj->pwm->TZSEL.bit.CBC1 = aParams->reg.TZSEL.bit.CBC1;
  obj->pwm->TZSEL.bit.CBC2 = aParams->reg.TZSEL.bit.CBC2;
  obj->pwm->TZSEL.bit.CBC3 = aParams->reg.TZSEL.bit.CBC3;
  obj->pwm->TZSEL.bit.OSHT1 = aParams->reg.TZSEL.bit.OSHT1;
  obj->pwm->TZSEL.bit.OSHT2 = aParams->reg.TZSEL.bit.OSHT2;
  obj->pwm->TZSEL.bit.OSHT3 = aParams->reg.TZSEL.bit.OSHT3;

  // if TZA or TZB actions have been defined, we force PWM to off,
  // the powerstage protection block will then handle PWM enabling/disabling
  if ((obj->pwm->TZCTL.all & 0xF) != 0xF) {
    obj->pwm->TZFRC.bit.OST = 1;
    // For whatever reason this disable fails sometimes unless called twice in a
    // row Specific to F28P65x target...
#ifdef TARGET_28p65x
    obj->pwm->TZFRC.bit.OST = 1;
#endif
  }
  EDIS;

  obj->sequence = 1;
}

void PLX_PWM_scalePeriod(PLX_PWM_Handle_t aHandle, float aScalingFactor) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  PLX_ASSERT(aScalingFactor > 0.0);

  float newTBPRD;
  if (obj->pwm->TBCTL.bit.CTRMODE == TB_COUNT_UPDOWN) {
    // up-down
    newTBPRD = (float)(obj->nomTBPRD) * aScalingFactor;
  } else {
    // saw-tooth
    newTBPRD = (float)(obj->nomTBPRD + 1) * aScalingFactor - 1.0;
  }
  if (newTBPRD < 1.0) {
    newTBPRD = 1.0;
  } else if (newTBPRD > 65535.0) {
    newTBPRD = 65535.0;
  }
  obj->pwm->TBPRD = (uint16_t)newTBPRD;
}

void PLX_PWM_setPhase(PLX_PWM_Handle_t aHandle, float aPhase) {
  PLX_PWM_Obj_t *obj = (PLX_PWM_Obj_t *)aHandle;

  if ((aPhase < 0) || (aPhase >= 1.0)) {
    aPhase = 0;
  }

  uint16_t phase;
  uint16_t dir;
  if (obj->pwm->TBCTL.bit.CTRMODE == TB_COUNT_UPDOWN) {
    // up-down
    if (aPhase <= 0.5) {
      phase = (uint16_t)((float)(obj->pwm->TBPRD) * (aPhase) * 2);
      dir = 0;
    } else {
      phase = (uint16_t)((float)(obj->pwm->TBPRD) * (1 - aPhase) * 2);
      dir = 1;
    }
  } else {
    // saw-tooth
    phase = (uint16_t)(((float)obj->pwm->TBPRD + 1) * (1 - aPhase));
    if (phase > obj->pwm->TBPRD) {
      phase = 0;
    }
    dir = 0;
  }
  obj->pwm->TBPHS.bit.TBPHS = phase;
  obj->pwm->TBCTL.bit.PHSDIR = dir;
}
