#include "main_control(1)/matrix_controller.h"


#include <iostream>
//пока подключим сюда консольный вывод, пока нет интерфейса


int main()
{
    MatrixController controller;

    // пока интерфейса нет, вручную создадим entry
    Entry entries[4];
    entries[0] = {0, 0, 5.0};
    entries[1] = {0, 3, -2.0};
    entries[2] = {2, 1, 7.0};
    entries[3] = {1, 0, 10.0};

    Matrix* matrix = nullptr;
    // вызываем: построить матрицу из вручную введенных элементов
    int result = controller.buildMatrix(5,5,4,entries,4,matrix);

    // тут должна быть проверка на ошибку

    std::cout << "start out\n";

    // небольщой вывод (пока тут) - это тест
    Colhead* col = matrix->first_col;
    while (col != nullptr)
    {
        std::cout << "col " << col->col+1 << ":";

        Node* node = col->first_el;
        while (node!=nullptr)
        {
            std::cout << "row " << node->row + 1 << " (" << node->value << "), ";
            node = node->next;
        }
        std::cout << "\n";
        col = col->next;

    }

    return 0;
};
