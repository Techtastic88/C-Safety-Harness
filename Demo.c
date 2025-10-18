#include "headers.h"

typedef struct safe {
    void **array;
    
    int elements;
    size_t array_s;
    size_t array_i;
    
    void* (*safe_f)(struct safe *obj); // constructor
    void* (*alloc_f)(struct safe *obj, uint64_t b); // fucntion
    void* (*freeall_f)(struct safe *obj); // desrtuctor
} safe_t;

void* alloc(safe_t *obj, uint64_t size) {
    
    void *memory = NULL;
    
    if (obj->elements > obj->array_i) { // check if theres space for another unit
        
        memory = malloc(size);
        
        if (memory != NULL) {
            *(obj->array + obj->array_i) = memory;
            obj->array_i++;
        } else {
            return alloc(obj, size);
        }
        
    } else { // if not make space for another unit
        
        obj->elements++;
        obj->array_s = sizeof(void*) * obj->elements;
        void *tmp = realloc(obj->array, obj->array_s);
        obj->array = tmp;
        
        memory = malloc(size);
        *(obj->array + obj->array_i) = memory;
        obj->array_i++;
    }
    
    return memory;
}

void* freeall(safe_t *obj) {
    for (int i = 0; i < obj->array_i; i++) {
        free(*(obj->array + i));
        *(obj->array + i) = NULL;
    }
    free(obj->array);
}

void* safe(safe_t *obj) {
    obj->elements = 8;
    obj->array_s = sizeof(void*) * obj->elements;
    obj->array_i = 0; 
    obj->array = malloc(obj->array_s);
    
    obj->alloc_f = alloc;
    obj->freeall_f = freeall;
}

int main(int argc, char *argv[]) {
    
    safe_t a;
    a.safe_f = safe;
    a.safe_f(&a);
    
    int *A = a.alloc_f(&a, 64);
    
    A[0] = 69;
    A[1] = 69;
    
    fprintf(stdout, "%d", A[0]);
    
    a.freeall_f(&a);
    
    return 0;
}
