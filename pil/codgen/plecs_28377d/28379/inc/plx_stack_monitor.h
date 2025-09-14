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

#ifndef PLX_STACKMON_H_
#define PLX_STACKMON_H_

#include "includes.h"

#if defined(__TI_EABI__)
extern uint16_t __stack, __TI_STACK_SIZE;
#else
extern uint16_t _stack, _STACK_SIZE;
#endif

#define PLX_STACK_FILL 0xAA55
#define PLX_STACK_PROTECTED_ENTRIES 150 // ~20% protected if stack size = 0x300
#define PLX_STACK_ENTRIES_PER_SCAN 10

typedef struct PLX_STACKMON_OBJ {
  uint16_t* highWatermark;
  uint16_t* stackStart;
  uintptr_t stackSize;
  uint16_t* scanPtr;
  float stackUsedPercent;
} PLX_StackMon_Obj_t;

typedef PLX_StackMon_Obj_t* PLX_StackMon_Handle_t;

extern PLX_StackMon_Handle_t PLX_StackMon_init(void* aMemory,
                                               const size_t aNumBytes);
extern void PLX_StackMon_paintStack(PLX_StackMon_Handle_t aHandle);
extern void PLX_StackMon_updateStackUseage(PLX_StackMon_Handle_t aHandle);
extern float PLX_StackMon_getStackUseagePercent(PLX_StackMon_Handle_t aHandle);

#endif /* PLX_STACKMON_H_ */
