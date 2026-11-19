#include "memory_control.h"

#include <iostream>

// ВОТ ЭТУ ХУЙНЮ ПИСАЛА ИИ, ПРОВЕРЯЙТЕ:)
// ВОТ ЭТУ ХУЙНЮ ПИСАЛА ИИ, ПРОВЕРЯЙТЕ:)
// ВОТ ЭТУ ХУЙНЮ ПИСАЛА ИИ, ПРОВЕРЯЙТЕ:)
// и вообще этот алгоритм надо обязательно оптимизировать
int insertElement(Matrix* matrix, int row, int col, double value)
{
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
        Colhead* newCol = new Colhead;

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

    // Теперь currentCol — нужный столбец.
    // Ищем место для строки.
    Node* previousNode = nullptr;
    Node* currentNode = currentCol->first_el;

    while (currentNode != nullptr && currentNode->row < row)
    {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    // Создаём новый Node
    Node* newNode = new Node;

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
    std::cout << "done insert\n";
    return 0;
}


int createMatrix(int rows, int cols, Matrix*& result)
{
    // не доверяем значениям! помните безопасное программирование
    if (rows < 1 || rows > 100000 ||
        cols < 1 || cols > 100000)
    {
        return -1; // код ошибки смотреть в файле
    }

    // создаем матрицу
    Matrix* matrix = new Matrix;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->nonZeroCount = 0;
    matrix->first_col = nullptr;
    // возвращаем
    result = matrix;

    std::cout << "memory alloc for matrix is ok\n";
    return 0; // операция выполнена.
}
// 2 return - плоховато, перепишите:(
