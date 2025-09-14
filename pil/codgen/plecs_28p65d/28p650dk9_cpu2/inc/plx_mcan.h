/*
   Copyright (c) 2022 by Plexim GmbH
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

#ifndef PLX_MCAN_H_
#define PLX_MCAN_H_

#include "includes.h"
#include "plx_mcan_impl.h" // implementation specific

#define PLX_MCAN_ESI_FLAG 0x0001
#define PLX_MCAN_FD_FLAG  0x0002
#define PLX_MCAN_BRS_FLAG 0x0004

extern void PLX_MCAN_sinit();

extern PLX_MCAN_Handle_t PLX_MCAN_init(void *aMemory, const size_t aNumBytes);

extern void PLX_MCAN_configure(PLX_MCAN_Handle_t aHandle, PLX_MCAN_Unit_t aUnit, const PLX_MCAN_Params_t *aParams);

extern void PLX_MCAN_setBusOn(PLX_MCAN_Handle_t aHandle, bool busOn);

extern bool PLX_MCAN_isBusOn(PLX_MCAN_Handle_t aHandle);

extern bool PLX_MCAN_isErrorActive(PLX_MCAN_Handle_t aHandle);

extern void PLX_MCAN_setupRxMailbox(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox, uint32_t aId, bool isExtended, unsigned char aDlc);

extern void PLX_MCAN_setupTxMailbox(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox, uint32_t aId, bool isExtended, unsigned char aDlc, bool aEnableBrs);

// not sure if needed...
extern bool PLX_MCAN_readyForTx(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox);

// not sure if needed...
extern void PLX_MCAN_getAndClearNewMsgFlag(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox);

extern bool PLX_MCAN_getMessage(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox, unsigned char data[], unsigned char lenMax, uint16_t *aFlags);

extern bool PLX_MCAN_putMessage(PLX_MCAN_Handle_t aHandle, uint16_t aMailBox, const unsigned char data[], unsigned char len);


#endif /* PLX_MCAN_H_ */
