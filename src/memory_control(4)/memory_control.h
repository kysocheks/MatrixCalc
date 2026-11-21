#pragma once

#include "../main_control(1)/matrix.h"

// создать матрицу без узлов
int createMatrix(int rows, int cols, Matrix*& result);

// создать и вставить узел
int insertElement(Matrix* matrix, int row, int col, double value);

// удалить матрицу
void deleteMatrix(Matrix*& matrix);
