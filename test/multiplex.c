#include <stdio.h>

#define NUM_BYTES 4
#define NUM_VARIABLES 64

void splitBytes(unsigned char* bytes, unsigned char* variables) {
    for (int i = 0; i < NUM_BYTES; i++) {
        for (int j = 0; j < 8; j++) {
            variables[i * 8 + j] = (bytes[i] & (1 << j)) >> j;
        }
    }
}

int main() {
    unsigned char bytes[NUM_BYTES] = {0x80, 0x01, 0xAB, 0x55};  // Example incoming bytes
    unsigned char variables[NUM_VARIABLES] = {0};

    splitBytes(bytes, variables);

    // Output the results
    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("bit%d: 0x%02X\n", i, variables[i]);
    }

    return 0;
}

