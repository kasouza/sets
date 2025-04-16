#ifndef KASOUZA_KLIB_DYNARR_H
#define KASOUZA_KLIB_DYNARR_H

#include <stdlib.h>
#include <assert.h>

#define KLIB_DYNARR_INITIAL_SIZE 8
#define KLIB_DYNARR_SCALE_FACTOR 2

#define KLIB_DYNARR(type) klib_Dynarr_##type

#define KLIB_DYNARR_DECLARE(type)                                              \
    typedef struct {                                                           \
        type *buffer;                                                          \
        int len;                                                               \
        int cap;                                                               \
    } KLIB_DYNARR(type);                                                       \
                                                                               \
    KLIB_DYNARR(type) * klib_create_dynarr_##type();

#define KLIB_DYNARR_DEFINE(type)                                               \
    KLIB_DYNARR(type) * klib_create_dynarr_##type() {                          \
        KLIB_DYNARR(type) *arr = malloc(sizeof(KLIB_DYNARR(type)));            \
        arr->buffer = malloc(sizeof(type) * KLIB_DYNARR_INITIAL_SIZE);         \
        arr->cap = KLIB_DYNARR_INITIAL_SIZE;                                   \
        arr->len = 0;                                                          \
        return arr;                                                            \
    }

#define KLIB_DYNARR_CREATE(type) klib_create_dynarr_##type()

#define KLIB_DYNARR_FREE(dynarr)                                               \
    do {                                                                       \
        assert(dynarr != NULL);                                                \
        assert(dynarr->buffer != NULL);                                        \
        free(dynarr->buffer);                                                  \
        free(dynarr);                                                          \
    } while (0)

#define KLIB_DYNARR_PUSH_BACK(dynarr, value)                                   \
    do {                                                                       \
        assert(dynarr != NULL);                                                \
        assert(dynarr->buffer != NULL);                                        \
        if ((dynarr->len + 1) > dynarr->cap) {                                 \
            int new_cap = dynarr->cap * KLIB_DYNARR_SCALE_FACTOR;              \
            dynarr->buffer =                                                   \
                realloc(dynarr->buffer, sizeof(*dynarr->buffer) * new_cap);    \
            dynarr->cap = new_cap;                                             \
        }                                                                      \
        dynarr->buffer[dynarr->len] = value;                                   \
        dynarr->len++;                                                         \
    } while (0)

#define KLIB_DYNARR_APPEND(dynarr, other_dynarr)                               \
    do {                                                                       \
        assert(dynarr != NULL);                                                \
        assert(dynarr->buffer != NULL);                                        \
        assert(other_dynarr != NULL);                                          \
        assert(other_dynarr->buffer != NULL);                                  \
                                                                               \
        for (int i = 0; i < other_dynarr->len; i++) {                          \
            KLIB_DYNARR_PUSH_BACK(dynarr, other_dynarr->buffer[i]);            \
        }                                                                      \
    } while (0)

#endif
