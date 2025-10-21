#include "headers.h"
#include "Safety-Harness.h"

int main() {
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
