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

#ifndef PLX_AIN_IMPL_H_
#define PLX_AIN_IMPL_H_

#include "adc.h"
#include "f28p65x_adc.h"
#include "f28p65x_adc_defines.h"
#include "ain_type4.h"

#define HAS_INTERNAL_VREF

extern void ADC_setVREF(uint32_t base, ADC_ReferenceMode refMode,
                        ADC_ReferenceVoltage refVoltage);

/* Wrapper on TI's ADC_setVREF */
static void SetVrefWrapper(int module, bool useExternalVref, float vref) {
  if (useExternalVref) {
    ADC_setVREF(module, ADC_REFERENCE_EXTERNAL,
                0 /*ref ignored for external mode*/);
  } else {
    if (vref == 3.3f) {
      ADC_setVREF(module, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_3_3V);
    } else if (vref == 2.5f) {
      ADC_setVREF(module, ADC_REFERENCE_INTERNAL, ADC_REFERENCE_2_5V);
    } else {
      PLX_ASSERT(0);  // invalid configuration
    }
  }
}

#endif /* PLX_AIN_IMPL_H_ */
