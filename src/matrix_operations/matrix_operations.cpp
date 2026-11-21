#include "matrix_operations.h"
#include "../main_control(1)/error_codes.h"
#include "../memory_control(4)/memory_control.h"

#include <iostream>

int addMatrices(const Matrix* A, const Matrix* B, Matrix*& result)
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
                sum = nodeA->value + nodeB->value;

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