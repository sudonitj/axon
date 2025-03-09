#include "../../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/utils/config.h"

// Reading and Writing to a file
// r - Read, Write - w, Read & Write - w+, Append - a

FILE* open_file(const char* filename, const char* mode){
    FILE *file = fopen(filename, mode); // Automatically allocates memory in heap instead of stack, so it won't get invalidated. 
    // The pointer variable file itself is on the stack, but what it points to (the FILE structure) is on the heap.
    // When the open_file() function returns, only the local variable file is destroyed, but the actual FILE structure in heap memory remains intact
    if(file == NULL) fprintf(stderr, "Error opening file: %s\n", filename);
    return file;
}

void flush_stream(FILE *file){
    int c;
    // The file position is automatically advanced after each fgetc call. This happens internally within the fgetc function
    // The next call to fgetc will read from the new position
    while((c = fgetc(file)) != EOF){
        // printf("Character: %d, Position: %ld\n", c, ftell(filea));
        putchar(c);
    }
    rewind(file);
}

void copy_file(FILE* source, FILE* destination) {
    int c;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, destination);
    }
    rewind(source); // Rewind the position of the file pointer to the beginning of the file
}

void close_files(FILE *file[], int size){
    for (size_t i = 0; i < size; i++)
    {
        if(fclose(file[i]) != 0){
            fprintf(stderr, "Error closing file\n");
        }
    }
}

void init_state(const char* filename, char** state){
    FILE *file = open_file(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    };
    int c;
    for (size_t i = 0; i < STATE_SIZE; i++){
        for (size_t j = 0; j < STATE_SIZE; j++){
            c = fgetc(file);
            if(c == EOF){
                state[i][j] = 0;
                return;
            }
            else{
                state[i][j] = (char)c;
            }
        }  
    } 
    fclose(file);
}