#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"

unsigned char aes_key[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};

unsigned char aes_iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

size_t add_padding(unsigned char* data, size_t data_len, size_t buffer_size) {
    size_t padding_len = 16 - (data_len % 16);
    if (data_len + padding_len > buffer_size) {
        return 0;
    }
    
    for (size_t i = 0; i < padding_len; i++) {
        data[data_len + i] = (unsigned char)padding_len;
    }
    
    return data_len + padding_len;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }
    
    FILE* input = fopen(argv[1], "rb");
    if (!input) {
        printf("Ошибка: не удалось открыть файл %s\n", argv[1]);
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);
    
    size_t buffer_size = file_size + 16;
    unsigned char* buffer = (unsigned char*)malloc(buffer_size);
    if (!buffer) {
        fclose(input);
        return 1;
    }
    
    size_t bytes_read = fread(buffer, 1, file_size, input);
    fclose(input);
    
    size_t padded_size = add_padding(buffer, bytes_read, buffer_size);
    if (padded_size == 0) {
        free(buffer);
        return 1;
    }
    
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, aes_key, aes_iv);
    
    AES_CBC_encrypt_buffer(&ctx, buffer, padded_size);
    
    FILE* output = fopen(argv[2], "wb");
    if (!output) {
        free(buffer);
        return 1;
    }
    
    fwrite(buffer, 1, padded_size, output);
    fclose(output);
    
    printf("Файл зашифрован: %s\n", argv[2]);
    
    free(buffer);
    return 0;
}