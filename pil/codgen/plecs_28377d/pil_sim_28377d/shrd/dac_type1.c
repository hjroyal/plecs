/*
   Copyright (c) 2019 by Plexim GmbH
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
#include "plx_dac.h"

#pragma diag_suppress 112  // PLX_ASSERT(0) in switch statements

void PLX_DAC_sinit() {
  EALLOW;
#ifdef DACA_BASE
  CpuSysRegs.PCLKCR16.bit.DAC_A = 1;
#endif
#ifdef DACB_BASE
  CpuSysRegs.PCLKCR16.bit.DAC_B = 1;
#endif
#ifdef DACC_BASE
  CpuSysRegs.PCLKCR16.bit.DAC_C = 1;
#endif
  EDIS;
}

PLX_DAC_Handle_t PLX_DAC_init(void *aMemory, const size_t aNumBytes) {
  PLX_DAC_Handle_t handle;

  if (aNumBytes < sizeof(PLX_DAC_Obj_t)) {
    return ((PLX_DAC_Handle_t)NULL);
  }

  // set handle
  handle = (PLX_DAC_Handle_t)aMemory;

  return handle;
}

void PLX_DAC_configure(PLX_DAC_Handle_t aHandle, PLX_DAC_Unit_t aUnit,
                       bool useExternalVref, float aVref) {
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;

  switch (aUnit) {
#ifdef DACA_BASE
    case PLX_DAC_A:
      obj->dac = &DacaRegs;
      break;
#endif
#ifdef DACB_BASE
    case PLX_DAC_B:
      obj->dac = &DacbRegs;
      break;
#endif
#ifdef DACC_BASE
    case PLX_DAC_C:
      obj->dac = &DaccRegs;
      break;
#endif
    default:
      PLX_ASSERT(0);
  }
  EALLOW;

#ifdef TARGET_28p65x
  // 28P65x requires explicit setting of 2x gain mode
  // when using 3.3 internal reference.
  if (!useExternalVref && (aVref == 3.3)) {
    obj->dac->DACCTL.bit.MODE = 1;  // 2x -> range 0..3.3V
  }
#endif
  // VDAC is an external voltage reference for the DACs,
  // we do not currently support it.
  obj->dac->DACCTL.bit.DACREFSEL = 1;  // 1 = VREFHI, 0 = REFERENCE_VDAC
  obj->dac->DACVALS.all = 0;
  obj->dac->DACOUTEN.bit.DACOUTEN = 1;
  EDIS;

  obj->voltsPerDacBit = aVref / 4096.0f;
}

void PLX_DAC_configureScaling(PLX_DAC_Handle_t aHandle, float aScale,
                              float aOffset, float aMin, float aMax) {
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;
  float voltsPerDacBit = obj->voltsPerDacBit;

  obj->scale = aScale / voltsPerDacBit;
  obj->offset = aOffset / voltsPerDacBit;
  obj->min = aMin / voltsPerDacBit;
  obj->max = aMax / voltsPerDacBit;
}

void PLX_DAC_setValF(PLX_DAC_Handle_t aHandle, float aVal) {
  PLX_DAC_Obj_t *obj = (PLX_DAC_Obj_t *)aHandle;

  uint16_t dacV = (uint16_t)(aVal * obj->scale + obj->offset);

  if (dacV > obj->max) {
    dacV = obj->max;
  } else if (dacV < obj->min) {
    dacV = obj->min;
  }
  PLX_DAC_setVal(aHandle, dacV);
}
