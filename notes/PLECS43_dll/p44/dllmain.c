/**
 * @brief vscode+cmake 编译dll
 */

// dllmain.c : 定义 DLL 应用程序的入口点。
#include "DllHeader.h"

void plecsSetSizes(struct SimulationSizes *aSizes) {
  aSizes->numInputs = 1;
  aSizes->numOutputs = 1;
  aSizes->numStates = 1;
  aSizes->numParameters = 0;
}

void plecsOutput(struct SimulationState *aState) {
#if 1
  aState->outputs[0] = aState->states[0];
  aState->states[0] = aState->outputs[0] + aState->inputs[0];
#else
  aState->states[0] = aState->outputs[0] + aState->inputs[0];
  aState->outputs[0] = aState->states[0];
#endif
}
void plecsStart(struct SimulationState *aState) { aState->states[0] = 0; }