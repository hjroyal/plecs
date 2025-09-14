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

#ifndef PLX_AIN_H_
#define PLX_AIN_H_

#include "includes.h"
#include "pil.h"
#include "plx_ain_impl.h" // implementation specific

extern void PLX_AIN_sinit(uint32_t aSysClkHz);

extern PLX_AIN_Handle_t PLX_AIN_init(void *aMemory, const size_t aNumBytes);

extern void PLX_AIN_setDefaultAdcParams(PLX_AIN_AdcParams_t *aParams, bool useExternalVref, float aVref, bool sigmodeDifferential);

extern void PLX_AIN_configure(PLX_AIN_Handle_t aHandle, PLX_AIN_Unit_t aUnit, const PLX_AIN_AdcParams_t *aParams);

extern void PLX_AIN_resetChannelSetup(PLX_AIN_Handle_t aHandle);

extern void PLX_AIN_setDefaultChannelParams(PLX_AIN_ChannelParams_t *aParams);

extern void PLX_AIN_setupChannel(PLX_AIN_Handle_t aHandle, uint16_t aChannel, uint16_t aSource, const PLX_AIN_ChannelParams_t *aParams);

extern float PLX_AIN_getInF(PLX_AIN_Handle_t aHandle, uint16_t aChannel);

#endif /* PLX_AIN_H_ */
