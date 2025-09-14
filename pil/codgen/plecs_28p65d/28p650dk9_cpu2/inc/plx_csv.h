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

#ifndef PLX_CSV_H_
#define PLX_CSV_H_

#include "includes.h"
#include "plx_sci.h"
#include "string.h"

enum PLX_CSV_states {
  PLX_CSV_SWAP_READY,
  PLX_CSV_CONVERT_READY,
  PLX_CSV_OUTPUT_READY,
};

// breaking convention here and having the type in lowercase --
// it makes dynamically selecting one of these options 
// during codegen much nicer!
enum PLX_CSV_dataType {
  PLX_CSV_bool,
  PLX_CSV_uint8_t,
  PLX_CSV_int8_t,
  PLX_CSV_uint16_t,
  PLX_CSV_int16_t,
  PLX_CSV_uint32_t,
  PLX_CSV_int32_t,
  PLX_CSV_float,
  PLX_CSV_double,
};

#define CSV_SEP_SIZE   3  // longest separator is tab (two chars) plus null
#define CSV_TERM_SIZE  3  // longest termiator is "\n\r" (two chars) plus null
#define CSV_STR_SIZE   32 // TODO justify
#define CSV_FMT_SIZE   4  // longest formatter string is "%lf" -- 3 chars + 1 null

union PLX_CSV_Input {
  bool      b;
  uint16_t  u8;   // 16 bits is smallest memory block on C2000
  int16_t   i8;   // same here
  uint16_t  u16;
  int16_t   i16;
  uint32_t  u32;
  int32_t   i32;
  float     f;
  double    d;
};

typedef struct PLX_CSV_STRINGBUFFER {
  char outBuffer[CSV_STR_SIZE];
  char formatter[CSV_FMT_SIZE];
  uint16_t inputType;
} PLX_CSV_stringBuffer_t;

typedef PLX_CSV_stringBuffer_t *PLX_CSV_sbHandle_t;

typedef struct PLX_CSV_OBJ {
  uint16_t state;
  char separator[CSV_SEP_SIZE];
  char terminator[CSV_TERM_SIZE];
  bool dataFresh;
  uint16_t numInputs;
  uint16_t currentOutput;
  uint16_t currentIndex;
  union PLX_CSV_Input *hotBuffer;
  union PLX_CSV_Input *coldBuffer;
  PLX_CSV_sbHandle_t  *strBuffers;
} PLX_CSV_Obj_t;

typedef PLX_CSV_Obj_t *PLX_CSV_Handle_t;

PLX_CSV_Handle_t PLX_CSV_init(void *aMemory, const size_t aNumBytes,
                              union PLX_CSV_Input *hotBuffer,
                              union PLX_CSV_Input *coldBuffer,
                              PLX_CSV_sbHandle_t *strBuffers);

void PLX_CSV_config(PLX_CSV_Handle_t handle, uint16_t numInputs, 
                    char* separator, char* terminator);

void PLX_CSV_captureInput(PLX_CSV_Handle_t handle, void* input, uint16_t index, size_t size);
void PLX_CSV_setDataFresh(PLX_CSV_Handle_t handle);
void PLX_CSV_swapBuffers(PLX_CSV_Handle_t handle);
void PLX_CSV_convertToString(PLX_CSV_Handle_t handle);
void PLX_CSV_transmitOutput(PLX_CSV_Handle_t csvHandle, int16_t sciChannel);
void PLX_CSV_configSignal(PLX_CSV_Handle_t handle, uint16_t inputNum, char* formatter, uint16_t type);
bool PLX_CSV_putStr(int16_t sciChannel, char* str, uint16_t *index);

#endif //PLX_CSV_H_
