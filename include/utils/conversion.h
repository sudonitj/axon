#ifndef UTILS_CONVERSION_H
#define UTILS_CONVERSION_H

char* bytes_to_hex(const unsigned char* data, size_t len);
char* hex_to_bytes(const char* hex_string, size_t* out_len);

#endif // UTILS_CONVERSION_H