#include <stdio.h>
#include "../../include/common/failures.h"
#include "../../include/utils/conversion.h"
#include <stdlib.h>
#include <string.h>

char* bytes_to_hex(const unsigned char* data, size_t len){
    char* hex = (char*)malloc(len*2 + 1);
    if(hex == NULL){
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    for (size_t i = 0; i < len; i++)
    {
        sprintf(hex + (i*2), "%02x", data[i]);
        // is a C library function that formats and stores a 
        // series of characters and values in a character array (string). 
        // It follows the same formatting rules as printf, but instead 
        // of writing to the console, it writes to a string.
    }
    hex[len*2] = '\0';
    return hex;
}

char* hex_to_bytes(const char* hex_string, size_t* out_len){
    size_t hex_len = strlen(hex_string);
    if (hex_len % 2 != 0) {
        fprintf(stderr, "Hex string must have even length\n");
        return NULL;
    }

    size_t bytes_len = hex_len / 2;
    *out_len = bytes_len;
    unsigned char* bytes = (unsigned char*)malloc(bytes_len);
    if (bytes == NULL) {
        fprintf(stderr, MEMORY_ALLOCATION_FAILURE);
        return NULL;
    }
    for (size_t i = 0; i < bytes_len; i++) {
        char hex_byte[3] = {hex_string[i*2], hex_string[i*2 + 1], '\0'};
        int value;
        /*A C library function that reads formatted input 
         from a string, similar to how scanf reads from standard 
         input but instead reads from a string.*/
         if (sscanf(hex_byte, "%x", &value) != 1) {
            fprintf(stderr, "Invalid hex string: %s\n", hex_byte);
            free(bytes);
            return NULL;
        }        
         bytes[i] = (unsigned char)value;
    }
    return bytes;
}