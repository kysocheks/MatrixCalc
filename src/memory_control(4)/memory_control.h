#pragma once

#include "../main_control(1)/matrix.h"
#include <fstream>

// создать матрицу без узлов
int createMatrix(int rows, int cols, Matrix*& result);

// создать и вставить узел
int insertElement(Matrix* matrix, int row, int col, double value);

// считать файл и создать узлы
int loadMatrixDirect(
    int rows,
    int cols,
    long long expected,
    std::ifstream& file,
    Matrix*& result
);

// удалить матрицу
void deleteMatrix(Matrix*& matrix);

