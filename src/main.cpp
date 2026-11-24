#include "main_control(1)/matrix_controller.h"
#include "main_control(1)/error_codes.h"
#include "memory_control(4)/memory_control.h"
#include "matrix_operations/matrix_operations.h"

#include <iostream>
#include <iomanip>
//пока подключим сюда консольный вывод, пока нет интерфейса

void tempOutput(Matrix *matrix)
{
        // небольшой вывод (пока тут) - это тест - ИИ
        std::cout << std::fixed << std::setprecision(1);
        for (int row = 0; row < matrix->rows; row++)
        {
            Colhead* col = matrix->first_col;

            for (int currentCol = 0; currentCol < matrix->cols; currentCol++)
            {
                double value = 0.0;

                // Ищем нужный столбец
                while (col != nullptr && col->col < currentCol)
                {
                    col = col->next;
                }

                // Если столбец существует, ищем нужную строку
                if (col != nullptr && col->col == currentCol)
                {
                    Node* node = col->first_el;

                    while (node != nullptr && node->row < row)
                    {
                        node = node->next;
                    }

                    if (node != nullptr && node->row == row)
                    {
                        value = node->value;
                    }
                }

                std::cout << value << "\t";
            }

            std::cout << "\n";
        }
}


int main()
{
    MatrixController controller;

    Matrix* matrix = nullptr;
    // тест генерации
    // вызываем: сгенерировать матрицу
    int result = controller.generateMatrix(100000,100000,1000000000,matrix);

    // тест ввода с клавиатуры
    //Entry entries[3];
    //entries[0] = {0, 2, 5.0};
    //entries[1] = {1, 2, -2.0};
    //entries[2] = {2, 1, 7.0};
    //int result = controller.buildMatrix(5,6,1,entries,3,matrix);

    // тест загрузки из файла
    //int result = controller.loadMatrixFromFile("test.txt", matrix);


    if (result == OK)
    {
        //tempOutput(matrix);
    }
    else
    {
        std::cout << "error code: " << result << "\n";
    }


    Matrix *sumResult = nullptr;
    int sumCode = addMatrices(matrix, matrix, sumResult);

    if (sumCode == OK)
    {
        std::cout << "result: \n";
        //tempOutput(sumResult);
    }
    else
    {
        std::cout << "error code: " << sumCode << "\n";
    }


    deleteMatrix(matrix);
    deleteMatrix(sumResult);


    return 0;
};
