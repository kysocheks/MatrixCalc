#include "matrix_operations.h"
#include "../main_control(1)/error_codes.h"
#include "../memory_control(4)/memory_control.h"

#include <iostream>
#include <cmath>

// временно
#include "../main_control(1)/progress.h"
/*
int addMatrices(const Matrix* A, const Matrix* B, Matrix*& result)
{
    int code = OK;
    result = nullptr;

    // Проверка входных данных
    if (A == nullptr || B == nullptr)
    {
        code = INVALID_ARGUMENT;
    }
    else if (A->rows != B->rows || A->cols != B->cols)
    {
        code = INVALID_SIZE;
    }
    else
    {
        // Создание результирующей матрицы
        code = createMatrix(A->rows, A->cols, result);

        if (code == OK)
        {
            Colhead* colA = A->first_col;
            Colhead* colB = B->first_col;

            while ((colA != nullptr || colB != nullptr) && code == OK)
            {
                int currentCol;
                Node* nodeA = nullptr;
                Node* nodeB = nullptr;

                // Выбираем текущий столбец
                if (colB == nullptr ||
                    (colA != nullptr && colA->col < colB->col))
                {
                    // Столбец есть только в A
                    currentCol = colA->col;
                    nodeA = colA->first_el;

                    colA = colA->next;
                }
                else if (colA == nullptr ||
                         (colB != nullptr && colB->col < colA->col))
                {
                    // Столбец есть только в B
                    currentCol = colB->col;
                    nodeB = colB->first_el;

                    colB = colB->next;
                }
                else
                {
                    // Столбец есть и в A, и в B
                    currentCol = colA->col;

                    nodeA = colA->first_el;
                    nodeB = colB->first_el;

                    colA = colA->next;
                    colB = colB->next;
                }

                // Обрабатываем элементы выбранного столбца
                while ((nodeA != nullptr || nodeB != nullptr) &&
                       code == OK)
                {
                    int currentRow;
                    double sum;

                    if (nodeA == nullptr)
                    {
                        // Элемент есть только в B
                        currentRow = nodeB->row;
                        sum = nodeB->value;

                        nodeB = nodeB->next;
                    }
                    else if (nodeB == nullptr)
                    {
                        // Элемент есть только в A
                        currentRow = nodeA->row;
                        sum = nodeA->value;

                        nodeA = nodeA->next;
                    }
                    else if (nodeA->row < nodeB->row)
                    {
                        // Элемент есть только в A
                        currentRow = nodeA->row;
                        sum = nodeA->value;

                        nodeA = nodeA->next;
                    }
                    else if (nodeB->row < nodeA->row)
                    {
                        // Элемент есть только в B
                        currentRow = nodeB->row;
                        sum = nodeB->value;

                        nodeB = nodeB->next;
                    }
                    else
                    {
                        // Элемент есть и в A, и в B
                        currentRow = nodeA->row;
                        sum = nodeA->value + nodeB->value;

                        nodeA = nodeA->next;
                        nodeB = nodeB->next;
                    }

                    // Нулевые элементы в разреженной матрице не храним
                    if (sum != 0.0)
                    {
                        code = insertElement(
                            result,
                            currentRow,
                            currentCol,
                            sum
                        );
                    }
                }
            }
        }
    }

    // Если произошла ошибка — удаляем частично созданную матрицу
    if (code != OK)
    {
        deleteMatrix(result);
        result = nullptr;
    }

    return code;
}
*/

int addMatrices(const Matrix* A, const Matrix* B, Matrix*& result)
{
    int code = OK;
    result = nullptr;

    // Проверка входных данных
    if (A == nullptr || B == nullptr)
    {
        code = INVALID_ARGUMENT;
    }
    else if (A->rows != B->rows || A->cols != B->cols)
    {
        code = INVALID_SIZE;
    }
    else
    {
        std::cout << "[Сложение] Начало..." << std::endl;

        // Создание результирующей матрицы
        code = createMatrix(A->rows, A->cols, result);

        if (code == OK)
        {
            /*
             * Сначала определяем точное количество
             * непустых столбцов, которые будут обработаны.
             */
            long long totalCols = 0;

            Colhead* tempA = A->first_col;
            Colhead* tempB = B->first_col;

            while (tempA != nullptr || tempB != nullptr)
            {
                totalCols++;

                if (tempB == nullptr ||
                    (tempA != nullptr && tempA->col < tempB->col))
                {
                    tempA = tempA->next;
                }
                else if (tempA == nullptr ||
                         (tempB != nullptr && tempB->col < tempA->col))
                {
                    tempB = tempB->next;
                }
                else
                {
                    // Столбец есть и в A, и в B
                    tempA = tempA->next;
                    tempB = tempB->next;
                }
            }

            std::cout << "[Сложение] Непустых столбцов: "
                      << totalCols << std::endl;

            Colhead* colA = A->first_col;
            Colhead* colB = B->first_col;

            long long processedCols = 0;

            // Обновляем прогресс примерно 100 раз
            long long progressStep =
                std::max(1LL, totalCols / 100);

            while ((colA != nullptr || colB != nullptr) &&
                   code == OK)
            {
                int currentCol;
                Node* nodeA = nullptr;
                Node* nodeB = nullptr;

                // Выбираем текущий столбец
                if (colB == nullptr ||
                    (colA != nullptr && colA->col < colB->col))
                {
                    // Столбец есть только в A
                    currentCol = colA->col;
                    nodeA = colA->first_el;

                    colA = colA->next;
                }
                else if (colA == nullptr ||
                         (colB != nullptr && colB->col < colA->col))
                {
                    // Столбец есть только в B
                    currentCol = colB->col;
                    nodeB = colB->first_el;

                    colB = colB->next;
                }
                else
                {
                    // Столбец есть и в A, и в B
                    currentCol = colA->col;

                    nodeA = colA->first_el;
                    nodeB = colB->first_el;

                    colA = colA->next;
                    colB = colB->next;
                }

                // Обрабатываем элементы выбранного столбца
                while ((nodeA != nullptr || nodeB != nullptr) &&
                       code == OK)
                {
                    int currentRow;
                    double sum;

                    if (nodeA == nullptr)
                    {
                        // Элемент есть только в B
                        currentRow = nodeB->row;
                        sum = nodeB->value;

                        nodeB = nodeB->next;
                    }
                    else if (nodeB == nullptr)
                    {
                        // Элемент есть только в A
                        currentRow = nodeA->row;
                        sum = nodeA->value;

                        nodeA = nodeA->next;
                    }
                    else if (nodeA->row < nodeB->row)
                    {
                        // Элемент есть только в A
                        currentRow = nodeA->row;
                        sum = nodeA->value;

                        nodeA = nodeA->next;
                    }
                    else if (nodeB->row < nodeA->row)
                    {
                        // Элемент есть только в B
                        currentRow = nodeB->row;
                        sum = nodeB->value;

                        nodeB = nodeB->next;
                    }
                    else
                    {
                        // Элемент есть и в A, и в B
                        currentRow = nodeA->row;
                        sum = nodeA->value + nodeB->value;

                        nodeA = nodeA->next;
                        nodeB = nodeB->next;
                    }

                    // Нулевые элементы в разреженной матрице не храним
                    if (sum != 0.0)
                    {
                        code = insertElement(
                            result,
                            currentRow,
                            currentCol,
                            sum
                        );
                    }
                }

                // Текущий столбец полностью обработан
                processedCols++;

                if (processedCols % progressStep == 0 ||
                    processedCols == totalCols)
                {
                    printProgress(
                        "[Сложение]",
                        processedCols,
                        totalCols
                    );
                }
            }

            // Переход на новую строку после прогресса
            if (totalCols > 0)
            {
                std::cout << std::endl;
            }
        }
    }

    // Если произошла ошибка — удаляем частично созданную матрицу
    if (code != OK)
    {
        deleteMatrix(result);
        result = nullptr;

        std::cout << "[Сложение] Ошибка. Код: "
                  << code
                  << std::endl;
    }
    else
    {
        std::cout << "[Сложение] Завершено."
                  << std::endl;
    }

    return code;
}


int subtractMatrices(const Matrix* A, const Matrix* B, Matrix*& result)
{
    if (A == nullptr || B == nullptr)
    {
        return INVALID_ARGUMENT;
    }
    if (A->rows != B->rows || A->cols != B->cols)
    {
        return INVALID_SIZE;
    }

    int code = createMatrix(A->rows, A->cols, result);
    if (code != OK)
    {
        return code;
    }

    Colhead* colA = A->first_col;
    Colhead* colB = B->first_col;

    while (colA != nullptr || colB != nullptr)
    {
        int currentCol;
        Node* nodeA = nullptr;
        Node* nodeB = nullptr;
        // если столбец в A меньше чем в B, то берем столбец из A, иначе из B, если они равны, то берем оба
        if (colB == nullptr || (colA != nullptr && colA->col < colB->col))
        {
            currentCol = colA->col;
            nodeA = colA->first_el;
            colA = colA->next;
        }
        else if (colA == nullptr || (colB != nullptr && colB->col < colA->col))
        {
            currentCol = colB->col;
            nodeB = colB->first_el;
            colB = colB->next;
        }
        else
        {
            currentCol = colA->col;
            nodeA = colA->first_el;
            nodeB = colB->first_el;
            colA = colA->next;
            colB = colB->next;

        }

        while (nodeA != nullptr || nodeB != nullptr)
        {
            int currentRow;
            double sum;

            if (nodeA == nullptr)
            {
                currentRow = nodeB->row;
                sum = nodeB->value;
                nodeB = nodeB->next;
            }
            else if (nodeB == nullptr)
            {
                currentRow = nodeA->row;
                sum = nodeA->value;
                nodeA = nodeA->next;
            }
            else if (nodeA->row < nodeB->row)
            {
                currentRow = nodeB->row;
                sum = nodeB->value;
                nodeB = nodeB->next;
            }
            else if (nodeB->row < nodeA->row)
            {
                currentRow = nodeA->row;
                sum = nodeA->value;
                nodeA = nodeA->next;
            }
            else
            {
                currentRow = nodeA->row;
                sum = nodeA->value - nodeB->value;

                nodeA = nodeA->next;
                nodeB = nodeB->next;
            }

            if (sum != 0.0)
            {
                code = insertElement(result, currentRow, currentCol, sum);

                if (code != OK)
                {
                    deleteMatrix(result);
                    result = nullptr;
                    return code;
                }
            }


            // тут надо добавить проверку на sum = 0
            // удалять элемент из результирующей матрицы в таком случае
        }

    }

    return OK;
}

int multiplyMatrices(const Matrix* A, const Matrix* B, Matrix*& result)
{
    if (A == nullptr || B == nullptr)
    {
        return INVALID_ARGUMENT;
    }
    if (A->cols != B->rows)
    {
        return INVALID_SIZE;
    }

    int code = createMatrix(A->rows, B->cols, result);
    if (code != OK)
    {
        return code;
    }

    double* sums = new (std::nothrow) double[A->rows]; // массив для хранения сумм

    Colhead** columnIndex = new (std::nothrow) Colhead * [A->cols]; // временный список указателей на ненулевые столбцы

    // мб проверка какая-то созданных массивов нужна?

    // инициализируем массив
    for (int i = 0; i < A->cols; i++)
    {
        columnIndex[i] = nullptr;
    }

    // проходимся по столбцам матрицы A и добавляем ссылки первые эл-ты в список временный
    Colhead* columnA = A->first_col;

    while (columnA != nullptr)
    {
        columnIndex[columnA->col] = columnA;
        columnA = columnA->next;
    }


    // Проходимся по столбцам матрицы B

    Colhead* columnB = B->first_col;

    while (columnB != nullptr)
    {
        // обнуляем суммы перед обходом нового столбца
        for (int i = 0; i < A->rows; i++)
        {
            sums[i] = 0;
        }


        // проходка по столбцу
        Node* nodeB = columnB->first_el;

        // горизонатальный цикл (охватывает все столбцы)
        while (nodeB != nullptr)
        {
            int k = nodeB->row;  // номер строки
            double v = nodeB->value; // значение элемента

            Colhead* currentColumnA = columnIndex[k]; // сразу получаем ссылку на нужный столбец BTW это оч круто да?

            if (currentColumnA != nullptr)
            {
                Node* nodeA = currentColumnA->first_el;

                while (nodeA != nullptr)
                {
                    int i = nodeA->row;

                    double term = nodeA->value * v;

                    // проверка получившегося произведения на корректность
                    if (!std::isfinite(term))
                    {
                        std::cout << "Ошибка в формировании результата.\n";
                        // хз какой щас код ошибки ставить, у нас с ними не так что-то
                    }

                    sums[i] += term;

                    // проверка получившейся суммы на корректность
                    if (!std::isfinite(sums[i]))
                    {
                        std::cout << "Ошибка в формировании результата.\n";
                        // хз какой щас код ошибки ставить, у нас с ними не так что-то
                    }

                    nodeA = nodeA->next;
                }
            }

            nodeB = nodeB->next;
        }

        // Записываем полученный столбец результата
        for (int i = 0; i < A->rows; i++)
        {
            if (sums[i] != 0)
            {
                code = insertElement(result, i, columnB->col, sums[i]);

                if (code != OK)
                {
                    std::cout << "Ошибка в встваке элемента в результирующий матрицу.\n";
                }
            }
        }

        columnB = columnB->next;

    }

    delete[] columnIndex;
    delete[] sums;

    return OK;
}

