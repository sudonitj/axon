#include "../include/utils/fileio.h"
#include <stdio.h>
#include <stdlib.h>

// stderr is a standard error stream in C, 
// which is used to output error messages. 
// Unlike stdout, which is buffered, stderr 
// is typically unbuffered, meaning that error
// messages are displayed immediately without 
// waiting for the buffer to fill up. 
// This ensures that error messages are printed 
// promptly, even if the program crashes 
// or terminates unexpectedly

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *source_file = NULL;
    FILE *destination_file = NULL;
    int status = EXIT_SUCCESS;

    source_file = open_file(argv[1], "r");
    if (source_file == NULL) {
        return EXIT_FAILURE;
    }

    destination_file = open_file(argv[2], "w");
    if (destination_file == NULL) {
        fclose(source_file);
        return EXIT_FAILURE;
    }

    flush_stream(source_file);
    copy_file(source_file, destination_file);
    
    FILE *files[] = {source_file, destination_file};
    close_files(files, 2);
    
    return status;
}