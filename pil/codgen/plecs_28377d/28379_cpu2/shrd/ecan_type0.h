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

#ifndef PLX_ECAN_TYPE0_H_
#define PLX_ECAN_TYPE0_H_

// TODO: PLX_ASSERT following in code
#define PLX_CANBUS_NUM_CHANNELS 32

typedef enum PLX_CANBUS_UNIT {
  PLX_CANBUS_CAN_A = 0,
#ifdef TARGET_2833x
  PLX_CANBUS_CAN_B
#endif
} PLX_CANBUS_Unit_t;

typedef struct PLX_CANBUS_PARAMS {
  uint16_t tseg1;
  uint16_t tseg2;
  uint16_t sjw;
  uint16_t sam;
  uint16_t brp;
  bool autoBusOn;
} PLX_CANBUS_Params_t;

typedef struct PLX_CANBUS_OBJ {
  struct ECAN_REGS eCanShadow;
  volatile struct MBOX *mailboxArray;
  volatile struct ECAN_REGS *eCanRegsPtr;
} PLX_CANBUS_Obj_t;

typedef PLX_CANBUS_Obj_t *PLX_CANBUS_Handle_t;

#endif /* PLX_ECAN_TYPE0_H_ */
