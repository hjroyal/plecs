/*
   Copyright (c) 2024 by Plexim GmbH
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

#include "plx_stack_monitor.h"

extern PLX_StackMon_Handle_t PLX_StackMon_init(void *aMemory,
                                               const size_t aNumBytes) {
  PLX_StackMon_Handle_t handle;

  if (aNumBytes < sizeof(PLX_StackMon_Obj_t)) {
    return ((PLX_StackMon_Handle_t)NULL);
  }

  // set handle
  handle = (PLX_StackMon_Handle_t)aMemory;

#if defined(__TI_EABI__)
  handle->stackStart = &__stack;
  handle->stackSize = (uintptr_t) &__TI_STACK_SIZE;
#else
  handle->stackStart = &_stack;
  handle->stackSize = (uintptr_t) &_STACK_SIZE;
#endif

  handle->highWatermark = handle->stackStart;
  handle->scanPtr = handle->stackStart + handle->stackSize - 1;
  handle->stackUsedPercent = 0.0;

  return handle;
}

/*
 * The Stack Monitor operates using the common "stack painting" method --
 * the stack is filled with a known value (in this case, 0xAA55), and then
 * the stack monitor searches for the highest address in the stack that has
 * been overwritten with a different value. This is assumed to be the
 * worst-case stack useage.
 * 
 * We reserve a certain number of entries at the bottom of the stack to NOT paint,
 * in order to avoid corrupting data already placed on the stack during init.
 * 
 * paintStack should ONLY be called once, during init -- any other useage has
 * a high risk of unrecoverably corrupting the stack.
 */
extern void PLX_StackMon_paintStack(PLX_StackMon_Handle_t aHandle) {

  uint16_t entriesToPaint = aHandle->stackSize - PLX_STACK_PROTECTED_ENTRIES;
  uint16_t* paintPtr = aHandle->stackStart + aHandle->stackSize - 1;

  while (entriesToPaint > 0) {
    *paintPtr = PLX_STACK_FILL;
    paintPtr--;
    entriesToPaint--;
  }
}

extern void PLX_StackMon_updateStackUseage(PLX_StackMon_Handle_t aHandle) {
  uint16_t loopCounter = 0;
  while ((*aHandle->scanPtr == PLX_STACK_FILL)
         && (aHandle->scanPtr != aHandle->stackStart)) {
    aHandle->scanPtr--;
    loopCounter++;
    if (loopCounter == PLX_STACK_ENTRIES_PER_SCAN) {
      return;  // return early to share time with other bgnd tasks
    }
  }

  if (aHandle->highWatermark < aHandle->scanPtr) {
    aHandle->highWatermark = aHandle->scanPtr;
  }

  aHandle->scanPtr = aHandle->stackStart + aHandle->stackSize - 1;

  float stackUsed = (float)(aHandle->highWatermark - aHandle->stackStart);
  aHandle->stackUsedPercent = 100.0 * stackUsed / (float)aHandle->stackSize;
}

extern float PLX_StackMon_getStackUseagePercent(PLX_StackMon_Handle_t aHandle) {
  return aHandle->stackUsedPercent;
}
