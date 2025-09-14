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

#ifndef QEP_H_
#define QEP_H_

typedef struct PLX_QEP_PARAMS {
  Uint32 QPOSINIT;          // Position Counter Init
  Uint32 QPOSMAX;           // Maximum Position Count
  union QEPCTL_REG QEPCTL;  // QEP Control
} PLX_QEP_Params_t;

typedef struct PLX_QEP_OBJ {
  volatile struct EQEP_REGS *qep;
} PLX_QEP_Obj_t;

typedef PLX_QEP_Obj_t *PLX_QEP_Handle_t;

inline void PLX_QEP_sinit() {
}

inline uint32_t PLX_QEP_getPosCnt(PLX_QEP_Handle_t aHandle) {
  PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;
  return obj->qep->QPOSCNT;
}

inline uint32_t PLX_QEP_getPosILatchCnt(PLX_QEP_Handle_t aHandle) {
  PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;
  return obj->qep->QPOSILAT;
}

inline bool PLX_QEP_getAndClearIndexFlag(PLX_QEP_Handle_t aHandle) {
  PLX_QEP_Obj_t *obj = (PLX_QEP_Obj_t *)aHandle;

  bool flag = obj->qep->QFLG.bit.IEL;
  obj->qep->QCLR.bit.IEL = flag;

  return flag;
}

#endif /* QEP_H_ */
