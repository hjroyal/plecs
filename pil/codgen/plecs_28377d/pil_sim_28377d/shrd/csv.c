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

#include "plx_csv.h"
#include "plx_hal.h"
#include "plx_dispatcher.h"
#include "pil.h"
#include "stdio.h"

PLX_CSV_Handle_t PLX_CSV_init(void *aMemory, const size_t aNumBytes,
                              union PLX_CSV_Input *hotBuffer, 
                              union PLX_CSV_Input *coldBuffer,
                              PLX_CSV_sbHandle_t *strBuffers) {
  PLX_CSV_Handle_t handle;

  if (aNumBytes < sizeof(PLX_CSV_Obj_t)) {
		return((PLX_CSV_Handle_t)NULL);
  }

	// set handle
	handle = (PLX_CSV_Handle_t)aMemory;

  handle->dataFresh = false;
  handle->state = PLX_CSV_SWAP_READY;

  handle->strBuffers = strBuffers;
  handle->hotBuffer = hotBuffer;
  handle->coldBuffer = coldBuffer;

	return handle;
}

void PLX_CSV_config(PLX_CSV_Handle_t handle, uint16_t numInputs, 
                    char* separator, char* terminator) {
  handle->numInputs = numInputs;

  memcpy(handle->separator, separator, strlen(separator));
  memcpy(handle->terminator, terminator, strlen(terminator));
}

void  PLX_CSV_captureInput(PLX_CSV_Handle_t handle, void* input, 
                           uint16_t index, size_t size) {
  memcpy(&handle->hotBuffer[index], input, size);
}

void PLX_CSV_setDataFresh(PLX_CSV_Handle_t handle) {
  handle->dataFresh = true;
}

void PLX_CSV_swapBuffers(PLX_CSV_Handle_t handle) {
  if ((handle->state == PLX_CSV_SWAP_READY) && (handle->dataFresh == true)) {
    static union PLX_CSV_Input *tempBuffPtr;

    uint16_t stat0 = PIL_DisableInt();

    tempBuffPtr = handle->hotBuffer;
    handle->hotBuffer = handle->coldBuffer;
    handle->coldBuffer = tempBuffPtr;

    PIL_RestoreInt(stat0);
    handle->dataFresh = false;
    handle->state = PLX_CSV_CONVERT_READY;
  }
}

void PLX_CSV_convertToString(PLX_CSV_Handle_t handle) {
  if (handle->state == PLX_CSV_CONVERT_READY)  {
    for (int i = 0; i < handle->numInputs; i++) {
      uint16_t stringLength = 0;
      switch (handle->strBuffers[i]->inputType) {
        case PLX_CSV_bool:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].b);
          break;
        case PLX_CSV_uint8_t:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].u8);    // casts to uint16_t on C2000
          break;
        case PLX_CSV_int8_t:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].i8);    // casts to int16_t on C2000
          break;
        case PLX_CSV_uint16_t:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].u16);
          break;
        case PLX_CSV_int16_t:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].i16);
          break;
        case PLX_CSV_uint32_t:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].u32);
          break;
        case PLX_CSV_int32_t:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].i32);
          break;
        case PLX_CSV_float:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
             handle->coldBuffer[i].f);
          break;
        case PLX_CSV_double:
          stringLength = sprintf(
            handle->strBuffers[i]->outBuffer,
            handle->strBuffers[i]->formatter,
            handle->coldBuffer[i].d);
          break;
        default:
          break;
      }
      if(i < (handle->numInputs-1)) {
        strcpy(&handle->strBuffers[i]->outBuffer[stringLength], handle->separator);
      } else {
        strcpy(&handle->strBuffers[i]->outBuffer[stringLength], handle->terminator);
      }
    }
    handle->state = PLX_CSV_OUTPUT_READY;
  }
}

void PLX_CSV_transmitOutput(PLX_CSV_Handle_t csvHandle, int16_t sciChannel) {
  if (csvHandle->state == PLX_CSV_OUTPUT_READY) {
    for (int i=csvHandle->currentOutput; i<csvHandle->numInputs; i++) {
      if(PLX_CSV_putStr(sciChannel, csvHandle->strBuffers[i]->outBuffer,
                        &csvHandle->currentIndex)) {
        continue;
      }
      else {
        csvHandle->currentOutput = i;
        return; // if putStr returns false, tx was busy and we need to retry
      }
    }
    csvHandle->currentOutput = 0;
    csvHandle->state = PLX_CSV_SWAP_READY;
  }
}

bool PLX_CSV_putStr(int16_t sciChannel, char* str, uint16_t *index) {
  for (int j=*index; j<strlen(str); j++) {
    if(!PLXHAL_SCI_txIsBusy(sciChannel)) {
      PLXHAL_SCI_putChar(sciChannel, str[j]);
    }
    else {
      *index = j;
      return false;
    }
  }
  *index = 0;
  return true;
}

void PLX_CSV_configSignal(PLX_CSV_Handle_t handle, uint16_t inputNum,
                          char* formatter, uint16_t type) {
  memcpy(handle->strBuffers[inputNum]->formatter, formatter, CSV_FMT_SIZE);
  handle->strBuffers[inputNum]->inputType = type;
}
