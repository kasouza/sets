#include "dynarr.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

KLIB_DYNARR_DECLARE(int);
KLIB_DYNARR_DEFINE(int);

struct hashset_node {
    int value;
    bool init;
};

struct hashset {
    struct hashset_node *buffer;
    int buffer_cap;
    int num_elements;
};

void add(struct hashset *set, int value);

uint32_t hash(int value, int cap) {
    uint32_t hash = value;
    return hash % cap;
}

void grow(struct hashset *set) {
    int new_cap = set->buffer_cap * 4;
    struct hashset_node *new_buffer =
        calloc(new_cap, sizeof(struct hashset_node));

    for (int i = 0; i < set->buffer_cap; i++) {
        if (set->buffer[i].init) {
            uint32_t hashed = hash(set->buffer[i].value, new_cap);
            uint32_t idx = hashed;

            while (new_buffer[idx].init) {
                if (idx >= new_cap - 1) {
                    idx = 0;
                } else {
                    idx++;
                }

                assert(idx != hashed);
            }

            assert(new_buffer[idx].init == false);
            new_buffer[idx] = set->buffer[i];
        }
    }

    free(set->buffer);
    set->buffer = new_buffer;
    set->buffer_cap = new_cap;
}

void add(struct hashset *set, int value) {
    float new_len = set->num_elements + 1;
    float load = new_len / set->buffer_cap;

    if (load > 0.5) {
        grow(set);
    }

    uint32_t hashed = hash(value, set->buffer_cap);
    uint32_t idx = hashed;

    while (set->buffer[idx].init) {
        if (set->buffer[idx].value == value) {
            return;
        }

        if (idx >= set->buffer_cap - 1) {
            idx = 0;
        } else {
            idx++;
        }

        assert(idx != hashed);
    }

    assert(set->buffer[idx].init == false);

    set->buffer[idx].value = value;
    set->buffer[idx].init = true;
    set->num_elements++;
}

void init_hashset(struct hashset *hashset) {
    hashset->buffer_cap = 8;
    hashset->num_elements = 0;
    hashset->buffer = calloc(hashset->buffer_cap, sizeof(struct hashset_node));
}

void destroy_hashset(struct hashset *hashset) {
    hashset->num_elements = 0;
    hashset->buffer_cap = 0;
    free(hashset->buffer);
}

struct optional_int {
    int value;
    bool init;
};

bool has(struct hashset *set, int value) {
    uint32_t hashed = hash(value, set->buffer_cap);
    uint32_t idx = hashed;

    while (set->buffer[idx].init) {
        if (set->buffer[idx].value == value) {
            return true;
        }

        if (idx >= set->buffer_cap - 1) {
            idx = 0;
        } else {
            idx++;
        }

        if (idx == hashed) {
            return false;
        }
    }

    return false;
}

KLIB_DYNARR(int) * intersection2(KLIB_DYNARR(int) * a, KLIB_DYNARR(int) * b) {
    struct hashset set;
    init_hashset(&set);

    KLIB_DYNARR(int) * smaller;
    KLIB_DYNARR(int) * larger;

    if (a->len < b->len) {
        smaller = a;
        larger = b;
    } else {
        smaller = b;
        larger = a;
    }

    for (int i = 0; i < smaller->len; i++) {
        add(&set, smaller->buffer[i]);
    }

    KLIB_DYNARR(int) *intersection = KLIB_DYNARR_CREATE(int);
    for (int i = 0; i < larger->len; i++) {
        if (has(&set, larger->buffer[i])) {
            KLIB_DYNARR_PUSH_BACK(intersection, larger->buffer[i]);
        }
    }

    destroy_hashset(&set);

    return intersection;
}

void intersection2_unique(KLIB_DYNARR(int) * a, KLIB_DYNARR(int) * b,
                          struct hashset *out_set) {
    struct hashset set;
    init_hashset(&set);

    KLIB_DYNARR(int) * smaller;
    KLIB_DYNARR(int) * larger;

    if (a->len < b->len) {
        smaller = a;
        larger = b;
    } else {
        smaller = b;
        larger = a;
    }

    for (int i = 0; i < smaller->len; i++) {
        add(&set, smaller->buffer[i]);
    }

    KLIB_DYNARR(int) *intersection = KLIB_DYNARR_CREATE(int);
    for (int i = 0; i < larger->len; i++) {
        if (has(&set, larger->buffer[i])) {
            add(out_set, larger->buffer[i]);
        }
    }

    destroy_hashset(&set);
}

int main() {
    srand(time(NULL));

    int maxIterations = 100000000;

    KLIB_DYNARR(int) *a = KLIB_DYNARR_CREATE(int);
    KLIB_DYNARR(int) *b = KLIB_DYNARR_CREATE(int);

    for (int i = 0; i < maxIterations; ++i) {

        // Maybe add only to a, maybe only to b, maybe to both
        if (rand() % 2) {
            int r = rand();
            KLIB_DYNARR_PUSH_BACK(a, r);
        }

        if (rand() % 2) {
            int r = rand();
            KLIB_DYNARR_PUSH_BACK(b, r);
        }

        // Add to both
        if (rand() % 2) {
            int r = rand();
            KLIB_DYNARR_PUSH_BACK(a, r);
            KLIB_DYNARR_PUSH_BACK(b, r);
        }
    }

    clock_t start = clock();

    KLIB_DYNARR(int) *result2 = intersection2(a, b);

    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    double time_spen_ms = time_spent * 1000;

    printf("Time taken for intersection2: %f ms\n", time_spen_ms);
    printf("Intersection2 result: %d\n", result2->len);

    // Time instersection2_unique
    /*struct hashset set;*/
    /*init_hashset(&set);*/
    /*intersection2_unique(a, b, &set);*/
    /*end = clock();*/
    /*time_spent = (double)(end - start) / CLOCKS_PER_SEC;*/
    /*time_spen_ms = time_spent * 1000;*/
    /*printf("Time taken for intersection2_unique: %f ms\n", time_spen_ms);*/
    /*printf("Intersection2 unique result: %d\n", set.num_elements);*/

    /*FILE *fp = fopen("result.txt", "w");*/
    /*if (fp == NULL) {*/
    /*perror("Failed to open file");*/
    /*return 1;*/
    /*}*/

    /*for (int i = 0; i < result2->len; i++) {*/
    /*fprintf(fp, "%d\n", result2->buffer[i]);*/
    /*}*/

    /*fclose(fp);*/

    /*FILE *fpu = fopen("result_unique.txt", "w");*/
    /*if (fpu == NULL) {*/
    /*perror("Failed to open file");*/
    /*return 1;*/
    /*}*/

    /*for (int i = 0; i < set.buffer_cap; i++) {*/
    /*if (set.buffer[i].init) {*/
    /*fprintf(fpu, "%d\n", set.buffer[i].value);*/
    /*}*/
    /*}*/

    /*destroy_hashset(&set);*/
    /*fclose(fpu);*/

    /*FILE *fp2 = fopen("a.txt", "w");*/
    /*if (fp2 == NULL) {*/
    /*perror("Failed to open file");*/
    /*return 1;*/
    /*}*/

    /*for (int i = 0; i < a->len; i++) {*/
    /*fprintf(fp2, "%d\n", a->buffer[i]);*/
    /*}*/

    /*fclose(fp2);*/

    /*FILE *fp3 = fopen("b.txt", "w");*/
    /*if (fp3 == NULL) {*/
    /*perror("Failed to open file");*/
    /*return 1;*/
    /*}*/

    /*for (int i = 0; i < b->len; i++) {*/
    /*fprintf(fp3, "%d\n", b->buffer[i]);*/
    /*}*/

    /*fclose(fp3);*/

    return 0;
}
