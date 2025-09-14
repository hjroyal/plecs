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

#ifndef PLX_IPC_H_
#define PLX_IPC_H_

#include "includes.h"
#include "plx_ipc_impl.h" // implementation specific

extern PLX_IPC_Handle_t PLX_IPC_init(void *aMemory, const size_t aNumBytes);

extern void PLX_IPC_setup(PLX_IPC_Handle_t aHandle, void* aAddr1, void* aAddr2);
extern void PLX_IPC_semaphoreSetup(PLX_IPC_Handle_t aHandle, volatile int16_t* aWriteSema, volatile int16_t* aReadSema, volatile int16_t* aLastWrittenSema);

extern void* PLX_IPC_startTx(PLX_IPC_Handle_t aHandle);
extern void PLX_IPC_stopTx(PLX_IPC_Handle_t aHandle);
extern void* PLX_IPC_startRx(PLX_IPC_Handle_t aHandle);
extern void PLX_IPC_stopRx(PLX_IPC_Handle_t aHandle);


#endif /* PLX_IPC_H_ */
