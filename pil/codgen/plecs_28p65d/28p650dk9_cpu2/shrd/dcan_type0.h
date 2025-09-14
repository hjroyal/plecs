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

#ifndef PLX_DCAN_TYPE0_H_
#define PLX_DCAN_TYPE0_H_

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_can.h"
#include "driverlib/can.h"

#define PLX_CANBUS_PADDING_VALUE 0xFF
// TODO: Assert following in code
#define PLX_CANBUS_NUM_CHANNELS 32

typedef struct PLX_CANBUS_PARAMS {
  uint16_t tseg1;
  uint16_t tseg2;
  uint16_t sjw;
  uint16_t sam;
  uint16_t brp;
  bool autoBusOn;
} PLX_CANBUS_Params_t;

typedef struct PLX_CANBUS_OBJ {
  uint32_t portHandle;
} PLX_CANBUS_Obj_t;

typedef PLX_CANBUS_Obj_t *PLX_CANBUS_Handle_t;

#endif /* PLX_DCAN_TYPE0_H_ */
