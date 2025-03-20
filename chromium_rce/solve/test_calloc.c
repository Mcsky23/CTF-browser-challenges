#include <stdio.h>

int main() {
    void *chunk1 = malloc(0x1000);
    void *chunk2 = malloc(0x30);
    free(chunk1);
}