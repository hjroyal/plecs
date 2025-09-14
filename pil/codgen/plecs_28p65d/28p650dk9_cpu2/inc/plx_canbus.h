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

#ifndef PLX_CANBUS_H_
#define PLX_CANBUS_H_

#include "includes.h"
#include "pil.h"
#include "plx_canbus_impl.h" // implementation specific

extern PLX_CANBUS_Handle_t PLX_CANBUS_init(void *aMemory, const size_t aNumBytes);

extern void PLX_CANBUS_configure(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_Unit_t aUnit, const PLX_CANBUS_Params_t *aParams);

extern void PLX_CANBUS_setBusOn(PLX_CANBUS_Handle_t aHandle, bool busOn);

extern bool PLX_CANBUS_isBusOn(PLX_CANBUS_Handle_t aHandle);

extern bool PLX_CANBUS_isErrorActive(PLX_CANBUS_Handle_t aHandle);

extern void PLX_CANBUS_setupMailbox(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox, bool aConfigureForTransmit, uint32_t aId, bool isExtended, unsigned char aLen);

extern void PLX_CANBUS_disableMailbox(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox);

extern bool PLX_CANBUS_getMessage(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox, unsigned char data[], unsigned char lenMax);

extern bool PLX_CANBUS_putMessage(PLX_CANBUS_Handle_t aHandle, uint16_t aMailBox, const unsigned char data[], unsigned char len);

// deprecated
extern void PLX_CANBUS_configureViaPinSet(PLX_CANBUS_Handle_t aHandle, PLX_CANBUS_Unit_t aUnit, uint16_t aPinset, const PLX_CANBUS_Params_t *aParams);

#endif /* PLX_CANBUS_H_ */
