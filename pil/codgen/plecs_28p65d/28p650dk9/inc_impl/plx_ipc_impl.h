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

#ifndef PLX_IPC_IMPL_H_
#define PLX_IPC_IMPL_H_

#include "driverlib/ipc.h"

typedef struct PLX_IPC_OBJ {
  uint16_t intr_state;
  void* addr[2];
  size_t numBytes;
  volatile int16_t* readBufferSema;
  volatile int16_t* writeBufferSema;
  volatile int16_t* lastWrittenSema;

} PLX_IPC_Obj_t;

typedef PLX_IPC_Obj_t* PLX_IPC_Handle_t;

#endif /* PLX_IPC_IMPL_H_ */
