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

#include "plx_ipc.h"

PLX_IPC_Handle_t PLX_IPC_init(void *aMemory, const size_t aNumBytes) {
  if (aNumBytes < sizeof(PLX_IPC_Obj_t)) {
    return ((PLX_IPC_Handle_t)NULL);
  }
  PLX_IPC_Handle_t handle = (PLX_IPC_Handle_t)aMemory;
  return handle;
}

void PLX_IPC_setup(PLX_IPC_Handle_t aHandle, void *aAddr1, void *aAddr2) {
  PLX_IPC_Obj_t *obj = (PLX_IPC_Obj_t *)aHandle;

  obj->addr[0] = aAddr1;
  obj->addr[1] = aAddr2;
}

void PLX_IPC_semaphoreSetup(PLX_IPC_Handle_t aHandle,
                            volatile int16_t *aWriteSema,
                            volatile int16_t *aReadSema,
                            volatile int16_t *aLastWrittenSema) {
  PLX_IPC_Obj_t *obj = (PLX_IPC_Obj_t *)aHandle;

  obj->writeBufferSema = aWriteSema;
  obj->readBufferSema = aReadSema;
  obj->lastWrittenSema = aLastWrittenSema;
}

inline void *PLX_IPC_startTx(PLX_IPC_Handle_t aHandle) {
  PLX_IPC_Obj_t *obj = (PLX_IPC_Obj_t *)aHandle;
  obj->intr_state = __disable_interrupts();
  switch (*obj->readBufferSema) {
    case 0:
      *obj->writeBufferSema = 1;
      break;
    case 1:
      *obj->writeBufferSema = 0;
      break;
    default:
      *obj->writeBufferSema = !*obj->lastWrittenSema;
  }
  return obj->addr[(*obj->writeBufferSema == 0) ? 0 : 1];
}

inline void PLX_IPC_stopTx(PLX_IPC_Handle_t aHandle) {
  PLX_IPC_Obj_t *obj = (PLX_IPC_Obj_t *)aHandle;
  *obj->lastWrittenSema = *obj->writeBufferSema;
  *obj->writeBufferSema = -1;
  __restore_interrupts(obj->intr_state);
}

inline void *PLX_IPC_startRx(PLX_IPC_Handle_t aHandle) {
  PLX_IPC_Obj_t *obj = (PLX_IPC_Obj_t *)aHandle;
  obj->intr_state = __disable_interrupts();
  switch (*obj->writeBufferSema) {
    case 0:
      *obj->readBufferSema = 1;
      break;
    case 1:
      *obj->readBufferSema = 0;
      break;
    default:
      *obj->readBufferSema = *obj->lastWrittenSema;
  }
  return obj->addr[(*obj->readBufferSema == 0) ? 0 : 1];
}

inline void PLX_IPC_stopRx(PLX_IPC_Handle_t aHandle) {
  PLX_IPC_Obj_t *obj = (PLX_IPC_Obj_t *)aHandle;
  *obj->readBufferSema = -1;
  __restore_interrupts(obj->intr_state);
}
