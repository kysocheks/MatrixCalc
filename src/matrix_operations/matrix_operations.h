#pragma once

#include "../main_control(1)/matrix.h"

int addMatrices(const Matrix* A, const Matrix* B, Matrix*& result);

int subtractMatrices(const Matrix* A, const Matrix* B, Matrix*& result);

int multiplyMatrices(const Matrix* A, const Matrix* B, Matrix*& result);