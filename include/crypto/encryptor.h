#ifndef CRYPTO_ENCRYPTOR_H
#define CRYPTO_ENCRYPTOR_H

#include <stdio.h>

char* encryptor(char** state, char* final_pass, int block_size);

#endif // CRYPTO_ENCRYPTOR_H