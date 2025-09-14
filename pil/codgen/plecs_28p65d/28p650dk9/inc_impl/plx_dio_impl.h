/*
   Copyright (c) 2014-2016 by Plexim GmbH
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

#ifndef PLX_DIO_IMPL_H_
#define PLX_DIO_IMPL_H_

typedef enum PLX_DIO_INPUTTYPE {
  PLX_DIO_NOPULL = 0,
  PLX_DIO_PULLUP
} PLX_DIO_InputType_t;

typedef struct {
  bool enableInvert;
  uint16_t qualPeriodInSysClk;
  uint16_t qualType;
  PLX_DIO_InputType_t type;
} PLX_DIO_InputProperties_t;

typedef enum PLX_DIO_OUTPUTTYPE {
  PLX_DIO_PUSHPULL = 0,
  PLX_DIO_OPENDRAIN
} PLX_DIO_OutputType_t;

typedef struct {
  bool enableInvert;
  PLX_DIO_OutputType_t type;
} PLX_DIO_OutputProperties_t;

typedef struct PLX_DIO_OBJ {
  uint32_t mask;
  bool activeHigh;
  volatile uint32_t *dat;
  volatile uint32_t *toggle;
} PLX_DIO_Obj_t;

typedef PLX_DIO_Obj_t *PLX_DIO_Handle_t;

inline bool PLX_DIO_get(PLX_DIO_Handle_t aHandle) {
  PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

  if (obj->activeHigh) {
    return ((*obj->dat & obj->mask) != 0);
  } else {
    return ((*obj->dat & obj->mask) == 0);
  }
}

inline void PLX_DIO_set(PLX_DIO_Handle_t aHandle, bool aVal) {
  PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

  if ((aVal && obj->activeHigh) || (!aVal && !obj->activeHigh)) {
    *obj->dat |= obj->mask;
  } else {
    *obj->dat &= ~obj->mask;
  }
}

inline void PLX_DIO_toggle(PLX_DIO_Handle_t aHandle) {
  PLX_DIO_Obj_t *obj = (PLX_DIO_Obj_t *)aHandle;

  *obj->toggle = obj->mask;
}

#endif /* PLX_DIO_IMPL_H_ */
