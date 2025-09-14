/*
   Copyright (c) 2018 by Plexim GmbH
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

#ifndef PLX_SCI_H_
#define PLX_SCI_H_

#include "includes.h"
#include "plx_sci_impl.h"  // implementation specific

extern PLX_SCI_Handle_t PLX_SCI_init(void *aMemory, const size_t aNumBytes);
extern void PLX_SCI_configure(PLX_SCI_Handle_t aHandle, PLX_SCI_Unit_t aUnit, uint32_t aClk);

extern bool PLX_SCI_setupPort(PLX_SCI_Handle_t aHandle, uint32_t baudrate);

extern bool PLX_SCI_rxReady(PLX_SCI_Handle_t aHandle);
extern bool PLX_SCI_txIsBusy(PLX_SCI_Handle_t aHandle);

extern uint16_t PLX_SCI_getChar(PLX_SCI_Handle_t aHandle);
extern void PLX_SCI_putChar(PLX_SCI_Handle_t aHandle, uint16_t c);

extern bool PLX_SCI_breakOccurred(PLX_SCI_Handle_t aHandle);
extern void PLX_SCI_reset(PLX_SCI_Handle_t aHandle);

// deprecated
extern void PLX_SCI_configureViaPinSet(PLX_SCI_Handle_t aHandle, uint16_t port, uint32_t clk);

#endif /* PLX_SCI_H_ */
