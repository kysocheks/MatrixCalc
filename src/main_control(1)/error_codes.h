#pragma once

enum MatrixError
{
    OK = 0,

    INVALID_SIZE = -1,
    INVALID_ARGUMENT = -2,
    DUPLICATE_ELEMENT = -3,
    MEMORY_ERROR = -4,
    DEGENERATE_MATIRX = -5, // МАТРИЦА ВЫРОЖДЕННАЯ
    INVALID_FILETYPE = -6, // НАРУШЕН ФОРМАТ ФАЙЛА
    IMPORT_ERROR = -7, // НЕ УДАЛОСЬ ОТКРЫТЬ ИЛИ ПРОЧИТАТЬ ФАЙЛ
    EXPORT_ERROR = -8, // НЕ УДАЛОСЬ ЗАПИСАТЬ ФАЙЛ
    //MEMORY_ERROR = -9, // НЕ УДАЛОСЬ ВЫДЕЛИТЬ ПАМЯТЬ
    //INVALID_ARGUMENT = -10, // ПРИ ВЫЧИСЛЕНИИ ПОЛУЧЕНО НЕДОПУСТИМОЕ ЧИСЛО
    INVALID_MATRIX = -11, // НУЖННАЯ МАТРИЦА ОТСУТСТВУЕТ 
    RESOURCES_ERROR = -12 // ПРЕВЫШЕНО ДОПУСТИМОЕ ОГРАНИЧЕНИЕ ПО РЕСУРСАМ
};
