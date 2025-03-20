#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

static void *(*real_malloc)(size_t) = NULL;
static void *(*real_calloc)(size_t, size_t) = NULL;
static void (*real_free)(void *) = NULL;

static int hooks_initialized = 0;

__attribute__((constructor))
static void init_hooks() {
    if (hooks_initialized) return;
    hooks_initialized = 1;

    real_malloc = dlsym(RTLD_NEXT, "malloc");
    real_calloc = dlsym(RTLD_NEXT, "calloc");
    real_free = dlsym(RTLD_NEXT, "free");

    if (!real_malloc || !real_calloc || !real_free) {
        fprintf(stderr, "Error in `dlsym`\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "init_hooks\n");
    fprintf(stderr, "real_malloc: %p\n", real_malloc);
    fprintf(stderr, "real_calloc: %p\n", real_calloc);
    fprintf(stderr, "real_free: %p\n", real_free);
}

int to_hex(size_t size, char *buf) {
    char hex_chars[] = "0123456789abcdef";
    int i = 0;
    size_t temp_size = size;
    do {
        buf[i++] = hex_chars[temp_size % 16];
        temp_size /= 16;
    } while (temp_size > 0);
    buf[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = buf[j];
        buf[j] = buf[i - j - 1];
        buf[i - j - 1] = temp;
    }
    return i;
}

void print_ptr_hex(void *ptr) {
    char buf[16];
    int len = to_hex((size_t)ptr, buf);
    write(1, "0x", 2);
    write(1, buf, len);
}

void *malloc(size_t size) {
    if (!hooks_initialized) init_hooks();
    void *ptr = real_malloc(size);
    // can't use printf here, because it calls malloc or similar
    char buf[16];
    int len = to_hex(size, buf);

    write(1, "malloc(0x", 9);
    write(1, buf, len);
    write(1, ") -> ", 5);
    print_ptr_hex(ptr);
    write(1, "\n", 1);

    return ptr;
}

void *calloc(size_t nmemb, size_t size) {
    if (!hooks_initialized) init_hooks();
    void *ptr = real_calloc(nmemb, size);
    char buf[16];
    int len = to_hex(nmemb, buf);

    write(1, "calloc(0x", 9);
    write(1, buf, len);
    write(1, ", ", 2);
    len = to_hex(size, buf);
    write(1, buf, len);
    write(1, ") -> ", 5);
    print_ptr_hex(ptr);
    write(1, "\n", 1);

    return ptr;
}

void free(void *ptr) {
    if (!hooks_initialized) init_hooks();
    write(1, "free(", 5);
    print_ptr_hex(ptr);
    write(1, ")\n", 2);
    real_free(ptr);
}

// gcc -shared -fPIC -o heap_trace.so heap_trace.c -ldl