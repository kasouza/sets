#ifndef KASOUZA_KLIB_HASH_MAP_H
#define KASOUZA_KLIB_HASH_MAP_H

#include <stdlib.h>

#define KLIB_HASH_MAP_BUCKETS_INITIAL_CAP 8
#define KLIB_HASH_MAP_BUCKETS_GROW_FACTOR 2
#define KLIB_HASH_MAP_MAX_LOAD_FACTOR 0.75

#define KLIB_HASH_MAP_BUCKET_NODES_INITIAL_CAP 8
#define KLIB_HASH_MAP_BUCKET_NODES_GROWTH_FACTOR 2

typedef size_t (*klib_hash_func_t)(void *, size_t);
typedef int (*klib_compare_func_t)(void *, void *);

// Bucket
#define KLIB_HASH_MAP_BUCKET(key_type, value_type)                             \
    klib_HashMapBucket_##key_type##_##value_type

#define KLIB_HASH_MAP_BUCKET_CREATE_FUNC(key_type, value_type)                 \
    klib_hash_map_##key_type##_##value_type##_bucket_create

#define KLIB_HASH_MAP_BUCKET_FREE_FUNC(key_type, value_type)                   \
    klib_hash_map_##key_type##_##value_type##_bucket_free

#define KLIB_HASH_MAP_BUCKET_SET_FUNC(key_type, value_type)                    \
    klib_hash_map_##key_type##_##value_type##_bucket_set

#define KLIB_HASH_MAP_BUCKET_GET_FUNC(key_type, value_type)                    \
    klib_hash_map_##key_type##_##value_type##_bucket_get

// Hash map
#define KLIB_HASH_MAP(key_type, value_type)                                    \
    klib_HashMap_##key_type##_##value_type

#define KLIB_HASH_MAP_NODE(key_type, value_type)                               \
    klib_HashMapNode_##key_type##_##value_type

#define KLIB_HASH_MAP_CREATE_FUNC(key_type, value_type)                        \
    klib_hash_map_##key_type##_##value_type##_create

#define KLIB_HASH_MAP_FREE_FUNC(key_type, value_type)                          \
    klib_hash_map_##key_type##_##value_type##_free

#define KLIB_HASH_MAP_SET_FUNC(key_type, value_type)                           \
    klib_hash_map_##key_type##_##value_type##_set

#define KLIB_HASH_MAP_GET_FUNC(key_type, value_type)                           \
    klib_hash_map_##key__type##_##value_type##_get

#define KLIB_HASH_MAP_GROW_FUNC(key_type, value_type)                          \
    klib_hash_map_##key_type##_##value_type##_grow

#define KLIB_HASH_MAP_DECLARE(key_type, value_type)                            \
    typedef struct {                                                           \
        key_type key;                                                          \
        value_type value;                                                      \
    } KLIB_HASH_MAP_NODE(key_type, value_type);                                \
                                                                               \
    typedef struct {                                                           \
        KLIB_HASH_MAP_NODE(key_type, value_type) * nodes;                      \
        size_t nodes_len;                                                      \
        size_t nodes_cap;                                                      \
        klib_compare_func_t compare_func;                                      \
    } KLIB_HASH_MAP_BUCKET(key_type, value_type);                              \
                                                                               \
    KLIB_HASH_MAP_BUCKET(key_type, value_type) *                               \
        KLIB_HASH_MAP_BUCKET_CREATE_FUNC(key_type, value_type)(                \
            klib_compare_func_t _compare_func);                                \
                                                                               \
    void KLIB_HASH_MAP_BUCKET_FREE_FUNC(key_type, value_type)(                 \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * bucket);                  \
                                                                               \
    void KLIB_HASH_MAP_BUCKET_SET_FUNC(key_type, value_type)(                  \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * bucket, key_type key,     \
        value_type value);                                                     \
                                                                               \
    value_type *KLIB_HASH_MAP_BUCKET_GET_FUNC(key_type, value_type)(           \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * bucket, key_type key);    \
                                                                               \
    typedef struct {                                                           \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * *buckets;                 \
        size_t number_of_elements;                                             \
        size_t buckets_cap;                                                    \
        klib_hash_func_t hash_func;                                            \
        klib_compare_func_t compare_func;                                      \
    } KLIB_HASH_MAP(key_type, value_type);                                     \
                                                                               \
    KLIB_HASH_MAP(key_type, value_type) *                                      \
        KLIB_HASH_MAP_CREATE_FUNC(key_type, value_type)();                     \
                                                                               \
    void KLIB_HASH_MAP_FREE_FUNC(key_type, value_type)(                        \
        KLIB_HASH_MAP(key_type, value_type) * map);                            \
                                                                               \
    void KLIB_HASH_MAP_GROW_FUNC(key_type, value_type)(                        \
        KLIB_HASH_MAP(key_type, value_type) * map, size_t new_capacity);       \
                                                                               \
    void KLIB_HASH_MAP_SET_FUNC(key_type, value_type)(                         \
        KLIB_HASH_MAP(key_type, value_type) * map, key_type key,               \
        value_type type);                                                      \
                                                                               \
    value_type *KLIB_HASH_MAP_GET_FUNC(key_type, value_type)(                  \
        KLIB_HASH_MAP(key_type, value_type) * map, key_type key);

#define KLIB_HASH_MAP_DEFINE(key_type, value_type, _hash_func, _compare_func)  \
    KLIB_HASH_MAP_BUCKET(key_type, value_type) *                               \
        KLIB_HASH_MAP_BUCKET_CREATE_FUNC(key_type, value_type)(                \
            klib_compare_func_t _compare_func) {                               \
        assert(_compare_func != NULL);                                         \
                                                                               \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) *bucket =                   \
            calloc(1, sizeof(KLIB_HASH_MAP_BUCKET(key_type, value_type)));     \
                                                                               \
        bucket->compare_func = _compare_func;                                  \
        bucket->nodes_len = 0;                                                 \
        bucket->nodes_cap = KLIB_HASH_MAP_BUCKET_NODES_INITIAL_CAP;            \
        bucket->nodes =                                                        \
            calloc(bucket->nodes_cap,                                          \
                   sizeof(KLIB_HASH_MAP_NODE(key_type, value_type)));          \
                                                                               \
        return bucket;                                                         \
    }                                                                          \
                                                                               \
    void KLIB_HASH_MAP_BUCKET_FREE_FUNC(key_type, value_type)(                 \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * bucket) {                 \
        assert(bucket != NULL);                                                \
        assert(bucket->nodes != NULL);                                         \
                                                                               \
        free(bucket->nodes);                                                   \
        free(bucket);                                                          \
    }                                                                          \
                                                                               \
    void KLIB_HASH_MAP_BUCKET_SET_FUNC(key_type, value_type)(                  \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * bucket, key_type key,     \
        value_type value) {                                                    \
        assert(bucket != NULL);                                                \
        assert(bucket->nodes != NULL);                                         \
        assert(bucket->compare_func != NULL);                                  \
        assert(bucket->nodes_len <= bucket->nodes_cap);                        \
                                                                               \
        for (int i = 0; i < bucket->nodes_len; i++) {                          \
            if (0 == bucket->compare_func(&bucket->nodes[i].key, &key)) {      \
                bucket->nodes[i].value = value;                                \
                return;                                                        \
            }                                                                  \
        }                                                                      \
                                                                               \
        size_t new_length = bucket->nodes_len + 1;                             \
        if (new_length >= bucket->nodes_cap) {                                 \
            size_t new_cap =                                                   \
                bucket->nodes_cap * KLIB_HASH_MAP_BUCKET_NODES_GROWTH_FACTOR;  \
            bucket->nodes = realloc(                                           \
                bucket->nodes,                                                 \
                sizeof(KLIB_HASH_MAP_NODE(key_type, value_type)) * new_cap);   \
            bucket->nodes_cap = new_cap;                                       \
        }                                                                      \
                                                                               \
        bucket->nodes[bucket->nodes_len].key = key;                            \
        bucket->nodes[bucket->nodes_len].value = value;                        \
        bucket->nodes_len++;                                                   \
    }                                                                          \
                                                                               \
    value_type *KLIB_HASH_MAP_BUCKET_GET_FUNC(key_type, value_type)(           \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) * bucket, key_type key) {   \
        assert(bucket != NULL);                                                \
        assert(bucket->nodes != NULL);                                         \
                                                                               \
        for (int i = 0; i < bucket->nodes_len; i++) {                          \
            if (0 == bucket->compare_func(&bucket->nodes[i].key, &key)) {      \
                return &bucket->nodes[i].value;                                \
            }                                                                  \
        }                                                                      \
                                                                               \
        return NULL;                                                           \
    }                                                                          \
                                                                               \
    KLIB_HASH_MAP(key_type, value_type) *                                      \
        KLIB_HASH_MAP_CREATE_FUNC(key_type, value_type)() {                    \
        assert(_hash_func != NULL);                                            \
        assert(_compare_func != NULL);                                         \
                                                                               \
        KLIB_HASH_MAP(key_type, value_type) *map =                             \
            calloc(1, sizeof(KLIB_HASH_MAP(key_type, value_type)));            \
                                                                               \
        map->number_of_elements = 0;                                           \
        map->buckets_cap = KLIB_HASH_MAP_BUCKETS_INITIAL_CAP;                  \
        map->buckets =                                                         \
            calloc(map->buckets_cap,                                           \
                   sizeof(KLIB_HASH_MAP_BUCKET(key_type, value_type)));        \
        map->hash_func = _hash_func;                                           \
        map->compare_func = _compare_func;                                     \
                                                                               \
        return map;                                                            \
    }                                                                          \
                                                                               \
    void KLIB_HASH_MAP_FREE_FUNC(key_type, value_type)(                        \
        KLIB_HASH_MAP(key_type, value_type) * map) {                           \
        assert(map != NULL);                                                   \
        assert(map->buckets != NULL);                                          \
                                                                               \
        for (size_t i = 0; i < map->buckets_cap; i++) {                        \
            if (map->buckets[i] != NULL) {                                     \
                KLIB_HASH_MAP_BUCKET_FREE(key_type, value_type,                \
                                          map->buckets[i]);                    \
            }                                                                  \
        }                                                                      \
                                                                               \
        free(map->buckets);                                                    \
        free(map);                                                             \
    }                                                                          \
                                                                               \
    void KLIB_HASH_MAP_GROW_FUNC(key_type, value_type)(                        \
        KLIB_HASH_MAP(key_type, value_type) * map, size_t new_capacity) {      \
        assert(map != NULL);                                                   \
        assert(map->buckets != NULL);                                          \
        assert(new_capacity > map->buckets_cap);                               \
                                                                               \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) **new_buckets = calloc(     \
            new_capacity, sizeof(KLIB_HASH_MAP_BUCKET(key_type, value_type))); \
                                                                               \
        for (int bucket_idx = 0; bucket_idx < map->buckets_cap;                \
             bucket_idx++) {                                                   \
            KLIB_HASH_MAP_BUCKET(key_type, value_type) *bucket =               \
                map->buckets[bucket_idx];                                      \
                                                                               \
            if (bucket == NULL) {                                              \
                continue;                                                      \
            }                                                                  \
                                                                               \
            for (int node_idx = 0; node_idx < bucket->nodes_len; node_idx++) { \
                KLIB_HASH_MAP_NODE(key_type, value_type) *node =               \
                    &bucket->nodes[node_idx];                                  \
                                                                               \
                size_t hash = map->hash_func(&node->key, map->buckets_cap);    \
                                                                               \
                if (new_buckets[hash] == NULL) {                               \
                    new_buckets[hash] = KLIB_HASH_MAP_BUCKET_CREATE(           \
                        key_type, value_type, map->compare_func);              \
                }                                                              \
                                                                               \
                KLIB_HASH_MAP_BUCKET_SET(key_type, value_type,                 \
                                         new_buckets[hash], node->key,         \
                                         node->value);                         \
            }                                                                  \
                                                                               \
            KLIB_HASH_MAP_BUCKET_FREE(key_type, value_type, bucket);           \
            map->buckets[bucket_idx] = NULL;                                   \
        }                                                                      \
                                                                               \
        free(map->buckets);                                                    \
        map->buckets_cap = new_capacity;                                       \
        map->buckets = new_buckets;                                            \
    }                                                                          \
                                                                               \
    void KLIB_HASH_MAP_SET_FUNC(key_type, value_type)(                         \
        KLIB_HASH_MAP(key_type, value_type) * map, key_type key,               \
        value_type value) {                                                    \
        assert(map != NULL);                                                   \
        double load = map->number_of_elements / (double)map->buckets_cap;      \
        if (load >= KLIB_HASH_MAP_MAX_LOAD_FACTOR) {                           \
            KLIB_HASH_MAP_GROW(                                                \
                key_type, value_type, map,                                     \
                map->buckets_cap *KLIB_HASH_MAP_BUCKETS_GROW_FACTOR);          \
        }                                                                      \
                                                                               \
        size_t hash = map->hash_func(&key, map->buckets_cap);                  \
        assert(hash < map->buckets_cap);                                       \
                                                                               \
        if (map->buckets[hash] == NULL) {                                      \
            map->buckets[hash] = KLIB_HASH_MAP_BUCKET_CREATE(                  \
                key_type, value_type, map->compare_func);                      \
        }                                                                      \
                                                                               \
        KLIB_HASH_MAP_BUCKET_SET(key_type, value_type, map->buckets[hash],     \
                                 key, value);                                  \
        map->number_of_elements++;                                             \
    }                                                                          \
                                                                               \
    value_type *KLIB_HASH_MAP_GET_FUNC(key_type, value_type)(                  \
        KLIB_HASH_MAP(key_type, value_type) * map, key_type key) {             \
        assert(map != NULL);                                                   \
        assert(map->buckets != NULL);                                          \
                                                                               \
        size_t hash = map->hash_func(&key, map->buckets_cap);                  \
        assert(hash < map->buckets_cap);                                       \
                                                                               \
        KLIB_HASH_MAP_BUCKET(key_type, value_type) *bucket =                   \
            map->buckets[hash];                                                \
        if (bucket == NULL) {                                                  \
            return NULL;                                                       \
        }                                                                      \
                                                                               \
        return KLIB_HASH_MAP_BUCKET_GET(key_type, value_type, bucket, key);    \
    }

// Bucket
#define KLIB_HASH_MAP_BUCKET_CREATE(key_type, value_type, _compare_func)       \
    KLIB_HASH_MAP_BUCKET_CREATE_FUNC(key_type, value_type)(_compare_func)

#define KLIB_HASH_MAP_BUCKET_FREE(key_type, value_type, bucket)                \
    KLIB_HASH_MAP_BUCKET_FREE_FUNC(key_type, value_type)(bucket)

#define KLIB_HASH_MAP_BUCKET_SET(key_type, value_type, bucket, key, value)     \
    KLIB_HASH_MAP_BUCKET_SET_FUNC(key_type, value_type)(bucket, key, value)

#define KLIB_HASH_MAP_BUCKET_GET(key_type, value_type, bucket, key)            \
    KLIB_HASH_MAP_BUCKET_GET_FUNC(key_type, value_type)(bucket, key)

// Hash map
#define KLIB_HASH_MAP_CREATE(key_type, value_type)                             \
    KLIB_HASH_MAP_CREATE_FUNC(key_type, value_type)()

#define KLIB_HASH_MAP_FREE(key_type, value_type, hash_map)                     \
    KLIB_HASH_MAP_FREE_FUNC(key_type, value_type)(hash_map)

#define KLIB_HASH_MAP_GROW(key_type, value_type, hash_map, new_capacity)       \
    KLIB_HASH_MAP_GROW_FUNC(key_type, value_type)(hash_map, new_capacity)

#define KLIB_HASH_MAP_SET(key_type, value_type, hash_map, key, value)          \
    KLIB_HASH_MAP_SET_FUNC(key_type, value_type)(hash_map, key, value)

#define KLIB_HASH_MAP_GET(key_type, value_type, hash_map, key)                 \
    KLIB_HASH_MAP_GET_FUNC(key_type, value_type)(hash_map, key)

#endif
