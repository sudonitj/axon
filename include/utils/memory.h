#ifndef UTILS_MEMORY_H
#define UTILS_MEMORY_H

#include <stdlib.h>
char** allocate_matrix_memory(size_t rows, size_t cols);
void free_matrix_memory(char** state, size_t rows);

#endif // UTILS_MEMORY_H