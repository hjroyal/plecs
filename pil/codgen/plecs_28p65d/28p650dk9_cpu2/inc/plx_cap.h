/*
   Copyright (c) 2020 by Plexim GmbH
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

#ifndef PLX_CAP_H_
#define PLX_CAP_H_

#include "includes.h"
#include "pil.h"
#include "plx_cap_impl.h"  // implementation specific

extern void PLX_CAP_sinit();

extern PLX_CAP_Handle_t PLX_CAP_init(void *aMemory, const size_t aNumBytes);

extern void PLX_CAP_setDefaultParams(uint16_t aNumEvents, bool aSingleShotOperation, PLX_CAP_Params_t *aParams);

extern void PLX_CAP_configure(PLX_CAP_Handle_t aHandle, uint16_t aUnit, uint16_t aGpio, const PLX_CAP_Params_t *aParams);

extern bool PLX_CAP_getNewValues(PLX_CAP_Handle_t aHandle, uint16_t aNewPrescale, uint32_t *aValues, bool *aOverflowFlag);

#endif /* PLX_CAP_H_ */
