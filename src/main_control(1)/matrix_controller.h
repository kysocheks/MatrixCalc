#include "matrix.h"


class MatrixController
{
public:
    int buildMatrix(
        int rows,
        int cols,
        long long expected,
        const Entry* entries,
        long long count, Matrix*& result
    );

    int generateMatrix(
        int rows,
        int cols,
        long long count,
        Matrix*& result
    );

};
