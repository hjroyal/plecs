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

#ifndef PLX_ECAP_TYPE3_H_
#define PLX_ECAP_TYPE3_H_

typedef struct PLX_CAP_REG_PARAMS {
  union ECCTL1_REG ECCTL1;
  union ECCTL2_REG ECCTL2;
} PLX_CAP_RegParams_t;

typedef struct PLX_CAP_PARAMS {
  PLX_CAP_RegParams_t reg;
} PLX_CAP_Params_t;

typedef struct PLX_CAP_OBJ {
  volatile struct ECAP_REGS *ecap;
  uint16_t numEvents;
  uint16_t eventMask;
} PLX_CAP_Obj_t;

typedef PLX_CAP_Obj_t *PLX_CAP_Handle_t;

inline void PLX_CAP_sinit() {
}

#endif /* PLX_ECAP_TYPE3_H_ */
