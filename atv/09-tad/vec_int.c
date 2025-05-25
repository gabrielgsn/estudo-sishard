#include <stdlib.h>
#include "vec_int.h"

vec_int *vec_int_create() {
    vec_int *v = malloc(sizeof(vec_int));
    if (v == NULL) {
        return NULL;
    }
    
    v->data = malloc(2 * sizeof(int));
    if (v->data == NULL) {
        free(v);
        return NULL;
    }
    
    v->size = 0;
    v->capacity = 2;
    
    return v;
}

void vec_int_destroy(vec_int **_v) {
    if (_v == NULL || *_v == NULL) {
        return;
    }
    free((*_v)->data);
    free(*_v);
    *_v = NULL;
}

int vec_int_size(vec_int *v) {
    if (v == NULL) {
        return -1;
    }
    return v->size;
}

int vec_int_at(vec_int *v, int pos, int *val) {
    if (v == NULL || pos < 0 || pos >= v->size) {
        return 0;
    }
    
    *val = v->data[pos];
    return 1;
}

int vec_int_insert(vec_int *v, int pos, int val) {
    if (v == NULL || pos < 0 || pos > v->size) {
        return 0;
    }
    
    if (v->size == v->capacity) {
        int new_capacity = v->capacity * 2;
        int *new_data = realloc(v->data, new_capacity * sizeof(int));
        if (new_data == NULL) {
            return 0;
        }
        v->data = new_data;
        v->capacity = new_capacity;
    }
    
    for (int i = v->size; i > pos; i--) {
        v->data[i] = v->data[i - 1];
    }
    
    v->data[pos] = val;
    v->size++;
    
    return 1;
}

int vec_int_remove(vec_int *v, int pos) {
    if (v == NULL || pos < 0 || pos >= v->size) {
        return 0;
    }
    
    for (int i = pos; i < v->size - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    
    v->size--;
    
    if (v->size > 0 && v->size == v->capacity / 4) {
        int new_capacity = v->capacity / 2;
        int *new_data = realloc(v->data, new_capacity * sizeof(int));
        if (new_data != NULL) {
            v->data = new_data;
            v->capacity = new_capacity;
        }
    }
    
    return 1;   
}
