#include "matrix_controller.h"
#include "../memory_control(4)/memory_control.h"

#include <iostream>

int MatrixController::buildMatrix(
    int rows,
    int cols,
    long long expected,
    const Entry* entries,
    long long count,
    Matrix*& result
)
{
    Matrix* matrix = nullptr;
    // создаем пустую матрицу в блоке хранения
    int createCode = createMatrix(rows, cols, matrix);

    // вот тут нужна проверка на ошибку

    // смотрим entries, заполняем матрицу
    for (long long i=0; i<count; i++)
    {
        int insertCode = insertElement(
            matrix,
            entries[i].row,
            entries[i].col,
            entries[i].value
        );
        // вот тут должна быть проверка на ошибку

    }

    result = matrix;
    std::cout << "matrix build is done\n";
    return 0;
}
