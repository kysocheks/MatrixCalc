#include "main_control(1)/matrix_controller.h"
#include "main_control(1)/error_codes.h"
#include "memory_control(4)/memory_control.h"
#include "matrix_operations/matrix_operations.h"

#include <iostream>
#include <iomanip>
//пока подключим сюда консольный вывод, пока нет интерфейса


int main()
{
    MatrixController controller;

    Matrix* matrix = nullptr;
    // тест генерации
    // вызываем: сгенерировать матрицу
    //int result = controller.generateMatrix(5,5,2,matrix);

    // тест ввода с клавиатуры
    Entry entries[3];
    entries[0] = {0, 2, 5.0};
    entries[1] = {1, 2, -2.0};
    entries[2] = {2, 1, 7.0};
    int result = controller.buildMatrix(5,6,1,entries,3,matrix);

    // тут должна быть проверка на ошибку


    if (result == OK)
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
        deleteMatrix(matrix);

    }
    else
    {
        std::cout << "error code: " << result << "\n";
    }



    // попытка сложения матриц
    // сначала создаем две матрицы руками (А и В)

    std::cout << "\n";
    std::cout << "\n";

    // A
    Matrix* A = nullptr;

    Entry entries_a[3];
    entries_a[0] = { 0, 0, 1.5 };
    entries_a[1] = { 0, 5, 7.8 };
    entries_a[2] = { 2, 1, 5.6 };

    int matrix_a = controller.buildMatrix(5, 6, 1, entries_a, 3, A);

    if (matrix_a != OK)
    {
        std::cout << "Error creating matrix A: " << matrix_a << "\n";
        return matrix_a;
    }

    // B

    Matrix* B = nullptr;

    Entry entries_b[3];
    entries_b[0] = { 0, 0, 3.2 };
    entries_b[1] = { 2, 2, 1.9 };
    entries_b[2] = { 4, 4, 7.3 };

    int matrix_b = controller.buildMatrix(5, 6, 1, entries_b, 3, B);

    if (matrix_b != OK)
    {
        std::cout << "Error creating matrix B: " << matrix_b << "\n";
        deleteMatrix(A);
        return matrix_b;
    }

    // C

    Matrix* C = nullptr;

    int res = addMatrices(A, B, C);

    if (res != OK)
    {
        std::cout << "Error adding matrices: " << res << "\n";
        deleteMatrix(A);
        deleteMatrix(B);
        return res;
    }

	// вывод спиздил сверху, надо сделать его отдельной функцией будет
	std::cout << std::fixed << std::setprecision(1);
	for (int row = 0; row < C->rows; row++)
	{
		Colhead* col = C->first_col;

		for (int currentCol = 0; currentCol < C->cols; currentCol++)
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
	deleteMatrix(C);



    return 0;
};
