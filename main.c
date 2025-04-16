#include "dynarr.h"
#include "hash_map.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

size_t hash(void *data, size_t size) {
    int x = *(int *)data;

    // TODO: FIGURE OUT WHY THIS MAKES THING NOT WORK?
    /*x = ((x >> 16) ^ x) * 0x45d9f3b;*/
    /*x = ((x >> 16) ^ x) * 0x45d9f3b;*/
    /*x = (x >> 16) ^ x;*/

    return x % size;
}

int compare(void *a, void *b) { return *(int *)a - *(int *)b; }

KLIB_DYNARR_DECLARE(int);
KLIB_DYNARR_DEFINE(int);

KLIB_HASH_MAP_DECLARE(int, int);
KLIB_HASH_MAP_DEFINE(int, int, hash, compare);

KLIB_DYNARR(int) * intersection(KLIB_DYNARR(int) * a, KLIB_DYNARR(int) * b) {
    KLIB_DYNARR(int) *c = KLIB_DYNARR_CREATE(int);

    for (int i = 0; i < a->len; i++) {
        for (int j = 0; j < b->len; j++) {
            if (a->buffer[i] == b->buffer[j]) {
                KLIB_DYNARR_PUSH_BACK(c, a->buffer[i]);
                break;
            }
        }
    }

    return c;
}

KLIB_DYNARR(int) * intersection2(KLIB_DYNARR(int) * a, KLIB_DYNARR(int) * b) {
    KLIB_HASH_MAP(int, int) *c = KLIB_HASH_MAP_CREATE(int, int);

    KLIB_DYNARR(int) *smaller;
    KLIB_DYNARR(int) *larger;

    if (a->len < b->len) {
        smaller = a;
        larger = b;
    } else {
        smaller = b;
        larger = a;
    }

    for (int i = 0; i < smaller->len; i++) {
        KLIB_HASH_MAP_SET(int, int, c, smaller->buffer[i], 1);
    }

    KLIB_DYNARR(int) *intersection = KLIB_DYNARR_CREATE(int);
    for (int i = 0; i < larger->len; i++) {
        if (KLIB_HASH_MAP_GET(int, int, c, larger->buffer[i]) != NULL) {
            KLIB_DYNARR_PUSH_BACK(intersection, larger->buffer[i]);
        }
    }

    KLIB_HASH_MAP_FREE(int, int, c);
    c = NULL;

    return intersection;
}

int compare_int(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void intersection3(int *a, int a_len, int *b, int b_len, int **out_c, int *out_c_len) {
    int min_len = a_len < b_len ? a_len : b_len;
    int c_cap = min_len;
    int *c = malloc(sizeof(int) * c_cap);
    int c_len = 0;

    int a_idx = 0;
    int b_idx = 0;

    while (a_idx < a_len && b_idx < b_len) {
        if (a[a_idx] == b[b_idx]) {
            c[c_len] = a[a_idx];
            a_idx++;
            b_idx++;
            c_len++;

        } else if (a[a_idx] < b[b_idx]) {
            a_idx++;
        } else {
            b_idx++;
        }
    }

    *out_c = c;
    *out_c_len = c_len;
}

int main() {
    srand(time(NULL));

    int maxIterations = 100000000;

    KLIB_DYNARR(int) *a = KLIB_DYNARR_CREATE(int);
    KLIB_DYNARR(int) *b = KLIB_DYNARR_CREATE(int);

    for (int i = 0; i < maxIterations; ++i) {
        KLIB_DYNARR_PUSH_BACK(a, i);
        KLIB_DYNARR_PUSH_BACK(b, i);
    }

    // Time the execution of intersection
    /*clock_t start = clock();*/
    /*KLIB_DYNARR(int) *result1 = intersection(a, b);*/
    /*clock_t end = clock();*/
    /*double time_spent = (double)(end - start) / CLOCKS_PER_SEC;*/
    /*double time_spen_ms = time_spent * 1000;*/
    /*printf("Time taken for intersection: %f ms\n", time_spen_ms);*/
    /*printf("Intersection result: %d\n", result1->len);*/

    // Time the execution of intersection2
    clock_t start = clock();
    /*KLIB_DYNARR(int) *result2 = intersection2(a, b);*/
    KLIB_DYNARR(int) *result2 = KLIB_DYNARR_CREATE(int);

    qsort(a->buffer, a->len, sizeof(int), compare_int);
    qsort(b->buffer, b->len, sizeof(int), compare_int);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    double time_spen_ms = time_spent * 1000;
    printf("Time taken for intersection2: %f ms\n", time_spen_ms);
    printf("Intersection2 result: %d\n", result2->len);



    // Time the execution of intersecion3
    int *result3 = NULL;
    int result3_len = 0;
    start = clock();
    intersection3(a->buffer, a->len, b->buffer, b->len, &result3, &result3_len);
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    time_spen_ms = time_spent * 1000;
    printf("Time taken for intersection3: %f ms\n", time_spen_ms);
    printf("Intersection3 result: %d\n", result3_len);

    free(result3);
    result3 = NULL;

    KLIB_DYNARR_FREE(a);
    a = NULL;

    KLIB_DYNARR_FREE(b);
    b = NULL;

    /*KLIB_DYNARR_FREE(result1);*/
    /*result1 = NULL;*/

    KLIB_DYNARR_FREE(result2);
    result2 = NULL;

    return 0;
}
