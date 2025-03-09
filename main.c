#include <stdio.h>
#include <stdlib.h>

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

// stderr is a standard error stream in C, 
// which is used to output error messages. 
// Unlike stdout, which is buffered, stderr 
// is typically unbuffered, meaning that error
// messages are displayed immediately without 
// waiting for the buffer to fill up. 
// This ensures that error messages are printed 
// promptly, even if the program crashes 
// or terminates unexpectedly

int main(int argc, const char* argv[]){
    int c;
    FILE *source_file = NULL;
    FILE *destination_file = NULL;
    int status = EXIT_SUCCESS;
    // These are predefined macros in <stdlib.h> that represent standard exit status codes

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