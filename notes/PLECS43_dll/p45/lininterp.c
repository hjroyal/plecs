/**
 * @brief 使用DLL模块实现线性插值功能
 */

// dllmain.c : 定义 DLL 应用程序的入口点。
#include "DllHeader.h"

#define LINEAR_INTERPOLATION(X1, Y1, X2, Y2, X)                                \
  ((Y2 - Y1) / (X2 - X1) * (X - X1) + Y1)

#define VECTOR_LENGTH (ParamDim(0, 0) * ParamDim(0, 1))

#define X(i) ParamRealData(0, i)
#define Y(i) ParamRealData(1, i)

double *x;
double *y;

int getIndex(double *x, int vectorLength, double xValue);

double lininterp(double *x, double *y, int vectorLength, double xValue) {
  int index = getIndex(x, vectorLength, xValue);
  if (index == vectorLength - 1)
    return LINEAR_INTERPOLATION(x[index - 1], y[index - 1], x[index], y[index],
                                xValue);
  else
    return LINEAR_INTERPOLATION(x[index], y[index], x[index + 1], y[index + 1],
                                xValue);
}

int getIndex(double *x, int vectorLength, double xValue) {
  int minIndex = 0;
  int maxIndex = vectorLength - 1;
  int middleIndex;

  if (xValue < x[minIndex])
    return minIndex;
  else if (xValue > x[maxIndex])
    return vectorLength - 1;

  while (1) {
    if (maxIndex - minIndex <= 1)
      return minIndex;
    middleIndex = (minIndex + maxIndex) / 2;
    if (x[middleIndex] > xValue)
      maxIndex = middleIndex;
    else
      minIndex = middleIndex;
  }
}

void plecsSetSizes(struct SimulationSizes *aSizes) {
  aSizes->numInputs = 1;
  aSizes->numOutputs = 1;
  aSizes->numStates = 0;
  aSizes->numParameters = 10;
}

void plecsOutput(struct SimulationState *aState) {
  aState->outputs[0] = lininterp(aState->parameters, aState->parameters+5,
                                 5, aState->inputs[0]);
}

