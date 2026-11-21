#include "memory_control.h"
#include "../main_control(1)/error_codes.h"

#include <new>




// и вообще этот алгоритм надо обязательно оптимизировать
int insertElement(Matrix* matrix, int row, int col, double value)
{
    int code = OK;
    // !!! по сути эту функцию вызывают только другие функции. поэтому все переданные
    // сюда значения должны быть верными. (при обязательной проверке их в вызывающей функции)
    // ПАМЯТКА ДЛЯ ДРУГОГО ЧЕЛОВЕКА - ЕСЛИ ВЫЗВАЛ insertElement, ПРОВЕРЬ ЗНАЧЕНИЯ ТАМ, ГДЕ ВЫЗЫВАЛ.

    // возможные проверки (если все же поставим)
    // if (matrix == nullptr)
    // if (row < 0 || row >= matrix->rows || col < 0 || col >= matrix->cols) // диапазоны


    // Ищем место для столбца
    Colhead* previousCol = nullptr;
    Colhead* currentCol = matrix->first_col;

    while (currentCol != nullptr && currentCol->col < col)
    {
        previousCol = currentCol;
        currentCol = currentCol->next;
    }

    // Столбца с таким номером ещё нет
    if (currentCol == nullptr || currentCol->col != col)
    {
        Colhead* newCol = new (std::nothrow) Colhead; // аккуратно выделяем память
        if (newCol == nullptr)  // сразу отлавливаем ошибку
        {
            code = MEMORY_ERROR;
        }
        else // иначе продолжаем работу
        {
            newCol->col = col;
            newCol->first_el = nullptr;

            // Вставляем новый столбец в начало
            if (previousCol == nullptr)
            {
                newCol->next = matrix->first_col;
                matrix->first_col = newCol;
            }
            // Вставляем между столбцами
            else
            {
                newCol->next = currentCol;
                previousCol->next = newCol;
            }
            currentCol = newCol;
        }
    }

    if (code == OK) // если стоблец успешно найден, без ошибок
    {
        // Ищем место для строки.
        Node* previousNode = nullptr;
        Node* currentNode = currentCol->first_el;
        while (currentNode != nullptr && currentNode->row < row)
        {
            previousNode = currentNode;
            currentNode = currentNode->next;
        }


        // Такой элемент уже существует
        if (currentNode != nullptr && currentNode->row == row)
        {
            code = DUPLICATE_ELEMENT;
        }
        else // элемента нет -> создаем
        {
            // Создаём новый Node (аккуратно)
            Node* newNode = new (std::nothrow) Node;
            if (newNode == nullptr)
            {
                code = MEMORY_ERROR;
            }
            else
            {
                newNode->row = row;
                newNode->value = value;

                // Вставляем Node в начало
                if (previousNode == nullptr)
                {
                    newNode->next = currentCol->first_el;
                    currentCol->first_el = newNode;
                }
                // Вставляем между Node
                else
                {
                    newNode->next = currentNode;
                    previousNode->next = newNode;
                }

                matrix->nonZeroCount++;
            }
        }
    }
    return code;
}


int createMatrix(int rows, int cols, Matrix*& result)
{
    // также вызывается другой функцией, значения должны быть проверены там
    int code = OK;
    // создаем матрицу
    Matrix* matrix = new (std::nothrow) Matrix;
    if (matrix == nullptr)
    {
        code = MEMORY_ERROR;
    }
    else
    {
        matrix->rows = rows;
        matrix->cols = cols;
        matrix->nonZeroCount = 0;
        matrix->first_col = nullptr;
        // возвращаем
        result = matrix;

    }
    return code;
}


void deleteMatrix(Matrix*& matrix)
{
    if (matrix != nullptr)
    {
        Colhead* currentCol = matrix->first_col;

        while (currentCol != nullptr)
        {
            // проходимся по каждому столбцу
            Colhead* nextCol = currentCol->next;
            Node* currentNode = currentCol->first_el;

            // проходимся по каждому элементу
            while (currentNode != nullptr)
            {
                Node* nextNode = currentNode->next;
                delete currentNode;
                currentNode = nextNode;
            }
            delete currentCol;

            currentCol = nextCol;
        }

        delete matrix;
        matrix = nullptr;
    }
}
