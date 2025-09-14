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

#ifndef PLX_QEP_H_
#define PLX_QEP_H_

#include "includes.h"
#include "pil.h"
#include "plx_qep_impl.h"  // implementation specific

extern void PLX_QEP_sinit();

extern PLX_QEP_Handle_t PLX_QEP_init(void *aMemory, const size_t aNumBytes);

extern void PLX_QEP_setDefaultParams(PLX_QEP_Params_t *aParams);

#ifdef PLX_QEP_USES_DRIVERLIB
// boards that use the driver lib configure pins directly
extern void PLX_QEP_configure(PLX_QEP_Handle_t aHandle, uint16_t aUnit, const PLX_QEP_Params_t *aParams);
#else
extern void PLX_QEP_configureWithPins(PLX_QEP_Handle_t aHandle, uint16_t aUnit, 
                          uint16_t aPin_a, uint16_t aPin_b, uint16_t aPin_i,
                          const PLX_QEP_Params_t *aParams);
#endif

extern uint32_t PLX_QEP_getPosCnt(PLX_QEP_Handle_t aHandle);

extern uint32_t PLX_QEP_getPosILatchCnt(PLX_QEP_Handle_t aHandle);

extern bool PLX_QEP_getAndClearIndexFlag(PLX_QEP_Handle_t aHandle);



#endif /* PLX_QEP_H_ */
