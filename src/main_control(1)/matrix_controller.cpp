#include "matrix_controller.h"
#include "error_codes.h"
#include "../memory_control(4)/memory_control.h"

#include <iostream>
#include <random>
#include <fstream>

// временно
#include "progress.h"

// эти функции уже вызываются интерфейсом, которому я бы не доверял
// мало ли что пользователь введет и интерфейс это пропустит
// здесь ведутся все проверки

int MatrixController::buildMatrix(
    int rows,
    int cols,
    long long expected,
    const Entry* entries,
    long long count,
    Matrix*& result
)
{
    int code = OK; // по умолчанию у нас все шикарно ваще
    result = nullptr;
    Matrix* matrix = nullptr;

    // Максимально возможное количество элементов
    long long total = static_cast<long long>(rows) * cols/10;
    if (count < 0 || count > total)
    {
        code = INVALID_SIZE; // оставить эту или добавить новую?
    }
    // границы матрицы
    else if (rows < 0 || rows > 100000 || cols < 0 || cols > 100000)
    {
        code = INVALID_SIZE;
    }
    else
    {
        // создаем пустую матрицу в блоке хранения
        int createCode = createMatrix(rows, cols, matrix);
        if (createCode != OK)   // ошибка create
        {
            code = createCode;
        }
        else
        {
            // смотрим entries, заполняем матрицу
            long long i = 0;
            while (i<count && code == OK) // вставляем только пока все без ошибок
            {
                if (entries[i].col < 0 || entries[i].col > matrix->cols ||
                    entries[i].row < 0 || entries[i].row > matrix->rows)
                {
                    code = INVALID_SIZE; // наверное стоит другой написать, новый на индексы
                }
                else
                {
                    int insertCode = insertElement(
                        matrix,
                        entries[i].row,
                        entries[i].col,
                        entries[i].value
                    );
                    // ошибка insert
                    if (insertCode != OK)
                    {
                        code = insertCode;
                    }
                    i++;
                }
            }
        }
    }

    // если произошла ошибка во время создания -> удалить матрицу
    if (code != OK)
    {
        deleteMatrix(matrix);
    }
    else
    {
        result = matrix;
    }

    return code;
}


int MatrixController::generateMatrix(
        int rows,
        int cols,
        long long count,
        Matrix*& result
)
{
    int code = OK;
    result = nullptr;
    Matrix* matrix = nullptr;

    // чтобы переполнения int не было перевод rows в long long перед работой
    long long total = static_cast<long long>(rows) * cols / 10;
    if (count < 0 || count > total)
    {
        return INVALID_SIZE;
    }
    // границы матрицы
    else if (rows < 0 || rows > 100000 || cols < 0 || cols > 100000)
    {
        code = INVALID_SIZE;
    }
    else
    {
        int createCode = createMatrix(rows, cols, matrix);
        // проверка на ошибку выделения памяти
        if (createCode != OK)
        {
            code = createCode;
        }
        else
        {
            //генерация...
            std::random_device rd; // источник случайности
            std::mt19937 gen(rd());// инициализация генератора

            std::uniform_int_distribution<int> rowDist(0, rows - 1);
            std::uniform_int_distribution<int> colDist(0, cols - 1);
            std::uniform_real_distribution<double> valueDist(
                -100.0,
                100.0 // !!!пока не понял как весь диапазон можно задать!!!
            );

            long long already_generated = 0;
            // заполнение
            while (already_generated < count && code == OK)
            {
                int row = rowDist(gen);
                int col = colDist(gen);
                double value = valueDist(gen);

                int insertCode = insertElement(matrix, row, col, value);
                // + временный вывод прогресса
                if (insertCode == OK) {already_generated++;printProgress("[Генерация]",already_generated,count);}
                else if (insertCode == DUPLICATE_ELEMENT) {} // это нормально
                else {code = insertCode;}    // какая либо другая ошибка
            }
        }
    }

    // если произошла ошибка во время создания -> удалить матрицу
    if (code != OK)
    {
        deleteMatrix(matrix);
    }
    else
    {
        result = matrix;
    }
    return code;
}



int MatrixController::loadMatrixFromFile(
    const std::string& filename,
    Matrix*& result
)
{
    int code = OK;
    result = nullptr;

    std::ifstream file;
    // открываем
    file.open(filename);
    if (!file.is_open())
    {
        code = IMPORT_ERROR;
    }
    else
    {
        int rows, cols;
        long long expected;

        // удалось ли считать 3 первых значения
        if (!(file >> rows >> cols >> expected))
        {
            code = INVALID_FILETYPE;
        }
        else //  прочли. теперь проверить эти значения
        {
            // проверка значений: строка столбец
            if (rows < 1 || rows > 100000 ||
                cols < 1 || rows > 100000)
            {
                code = INVALID_SIZE;
            }
            else // проверка значений: кол-во ненулевых эл.
            {
                long long total = static_cast<long long>(rows) * cols / 10;
                if (expected < 0 || expected > total)
                {
                    code = INVALID_SIZE;
                }
                else
                {
                    // значения первой строки верны. передаем управление блоку памяти.
                    // с верными значениями
                    code = loadMatrixDirect(rows, cols, expected, file, result);
                }
            }
        }
    }

    return code;

}
