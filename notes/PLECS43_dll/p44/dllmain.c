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

/**note
使用DLL模块实现离散传递函数

Y(z)/U(z) = 1/(z-1)

z*Y(z) - Y(z) = U(z)

Y(z) - z^(-1)Y(z) = z^(-1)U(z)  //z变换

y(n)- y(n-1) = u(n-1)


*/