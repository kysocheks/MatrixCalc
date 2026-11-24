#include "memory_control.h"
#include "../main_control(1)/error_codes.h"

#include <new>

#include <string>
#include <sstream>


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

int loadMatrixDirect(
    int rows,
    int cols,
    long long expected,
    std::ifstream& file,
    Matrix*& result
)
{
    int code = OK;
    result = nullptr;

    Matrix* matrix = nullptr;

    // Первый этап. Создать пустую матрицу
    int createCode = createMatrix(
        rows,
        cols,
        matrix
    );

    if (createCode != OK)
    {
        code = createCode;
    }
    else
    {
        Colhead* lastCol = nullptr; // указатель на последний столбец
        int prevCol = -1;            // предыдущий столбец
        long long loaded = 0;        // сколько реально загрузили

        std::string line;

        //---------------- Читаем файл построчно.  --------------------
        // Одна строка = один столбец.
        while (code == OK &&
               std::getline(file, line))
        {
            if (line.empty())
            {
                // Пропускаем пустую строку
            }
            else
            {
                // Поток для чтения текущей строки
                std::stringstream lineStream(line);

                int fileCol;

                // Считываем первое значение строки — столбец
                if (!(lineStream >> fileCol))
                {
                    code = INVALID_FILETYPE;
                }
                else if (fileCol < 1 || fileCol > cols)
                {
                    code = INVALID_SIZE;
                }
                else
                {
                    // В файле индексы с 1, в памяти с 0
                    int col = fileCol - 1;

                    // Файл должен быть отсортирован по столбцам
                    if (col <= prevCol)
                    {
                        code = INVALID_FILETYPE;
                    }
                    else
                    {
                        // Создаём новый столбец
                        Colhead* newCol =
                            new (std::nothrow) Colhead;

                        if (newCol == nullptr)
                        {
                            code = MEMORY_ERROR;
                        }
                        else
                        {
                            // Создаём структуру столбца
                            newCol->col = col;
                            newCol->first_el = nullptr;
                            newCol->next = nullptr;

                            // Вставка столбца в матрицу
                            if (matrix->first_col == nullptr)
                            {
                                // Первый столбец
                                matrix->first_col = newCol;
                            }
                            else
                            {
                                // Добавляем в конец
                                lastCol->next = newCol;
                            }

                            // Запоминаем последний столбец
                            lastCol = newCol;

                            // Работа с элементами текущего столбца
                            Node* lastNode = nullptr;

                            // Предыдущая строка
                            int prevRow = -1;

                            // Читаем пары:
                            // строка значение
                            while (code == OK) // --------- работа с элементами столбца ---
                            {
                                int fileRow;
                                double value;

                                // Если пары закончились,
                                // переходим к следующему столбцу
                                if (!(lineStream >> fileRow))
                                {
                                    break;
                                }

                                // После строки обязательно
                                // должно идти значение
                                if (!(lineStream >> value))
                                {
                                    code = INVALID_FILETYPE;
                                }
                                else if (fileRow < 1 ||
                                         fileRow > rows)
                                {
                                    code = INVALID_SIZE;
                                }
                                else
                                {
                                    // В файле с 1, в памяти с 0
                                    int row = fileRow - 1;

                                    // Строки должны быть отсортированы
                                    if (row <= prevRow)
                                    {
                                        code = INVALID_FILETYPE;
                                    }
                                    else
                                    {
                                        // Создаём новый элемент
                                        Node* newNode =
                                            new (std::nothrow) Node;

                                        if (newNode == nullptr)
                                        {
                                            code = MEMORY_ERROR;
                                        }
                                        else
                                        {
                                            newNode->row = row;
                                            newNode->value = value;
                                            newNode->next = nullptr;

                                            // Первый элемент столбца
                                            if (newCol->first_el == nullptr)
                                            {
                                                newCol->first_el = newNode;
                                            }
                                            else
                                            {
                                                // Добавляем в конец
                                                lastNode->next = newNode;
                                            }

                                            // Запоминаем последний элемент
                                            lastNode = newNode;

                                            // Запоминаем строку
                                            // для следующей проверки
                                            prevRow = row;

                                            // Увеличиваем количество
                                            loaded++;
                                        }
                                    }
                                }
                            }

                            // Работа со строкой завершена.
                            // В каждом столбце должен быть
                            // хотя бы один элемент.
                            if (code == OK &&
                                newCol->first_el == nullptr)
                            {
                                code = INVALID_FILETYPE;
                            }

                            // Запоминаем столбец для следующей
                            // проверки сортировки
                            prevCol = col;
                        }
                    }
                }

                // После последней пары "строка значение"
                // не должно остаться лишних данных.
                //
                // Например:
                // 1 2 5 3 7 piu piu
                //
                // является ошибкой.
                if (code == OK)
                {
                    lineStream >> std::ws;

                    if (!lineStream.eof())
                    {
                        code = INVALID_FILETYPE;
                    }
                }
            }
        }

        // ---------------- ЧТЕНИЕ ЗАВЕРШЕНО -----------
        // Если чтение завершилось не из-за EOF,
        // значит произошла ошибка чтения.
        if (code == OK &&
            !file.eof())
        {
            code = IMPORT_ERROR;
        }

        // Количество считанных элементов должно
        // совпадать с количеством из заголовка.
        if (code == OK &&
            loaded != expected)
        {
            code = INVALID_FILETYPE;
        }

        // Если всё прошло успешно,
        // передаём матрицу наружу.
        if (code == OK)
        {
            matrix->nonZeroCount = loaded;
            result = matrix;
        }
    }

    // При любой ошибке удаляем частично
    // созданную матрицу.
    if (code != OK)
    {
        deleteMatrix(matrix);
    }

    return code;
}
