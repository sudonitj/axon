#ifndef UTILS_FILEIO_H
#define UTILS_FILEIO_H

#include <stdio.h>

FILE* open_file(const char* filename, const char* mode);
void flush_stream(FILE *file);
void copy_file(FILE* source, FILE* destination);
void close_files(FILE *file[], int size);
void init_state(const char* filename, char** state);
#endif // UTILS_FILEIO_H