#include "progress.h"

#include <iostream>

void printProgress(
    const char* operation,
    long long current,
    long long total
)
{
    int percent = 0;

    if (total > 0)
    {
        percent = static_cast<int>(
            current * 100 / total
        );
    }

    std::cout << "\r"
              << operation
              << " "
              << percent
              << "% ("
              << current
              << " / "
              << total
              << ")"
              << std::flush;
}
