#include <stdio.h> //malloc
#include <stdlib.h> //malloc
#include <pthread.h> //threads

typedef struct {
    int **A;
    int **B;
    int x;
} data;

typedef struct {
    data *d;
    int i;
    int j;
    int result;
} arg;

void *calc(void* args);
void mat_mul(int **A, int **B, int r, int x, int c, int **C);

int main(int argc,  char*argv[]) {

    int i, j, r=2, x=3, c=2;
    int **A, **B, **C;
    int aData[2][3] = {
        {1, 2 ,3},
        {4, 5, 6}
    };
    int bData[3][2] = {
        {10, 11},
        {20, 21},
        {30, 31}
    };

    A = (int**)malloc(r*sizeof(int*));
    B = (int**)malloc(x*sizeof(int*));
    C = (int**)malloc(r*sizeof(int*));
    
    for(i = 0; i < r; i++) {
        A[i] = (int*)malloc(x*sizeof(int));
        for(j = 0; j < x; j++)
            A[i][j] = aData[i][j];
    }
        
    for(i = 0; i < x; i++) {
        B[i] = (int*)malloc(c*sizeof(int));
        for(j = 0; j < c; j++)
            B[i][j] = bData[i][j];
    }   

    for(i = 0; i < r; i++)
        C[i] = (int*)malloc(c*sizeof(int));

    mat_mul(A, B, r, x , c, C);

    // print results
    printf("\nA:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < x; j++)
            printf("%d\t", A[i][j]);
        printf("\n");    
    }
    printf("\nB:\n");
    for(i = 0; i < x; i++) {
        for(j = 0; j < c; j++)
            printf("%d\t", B[i][j]);
        printf("\n");    
    }
    printf("\nC:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++)
            printf("%d\t", C[i][j]);
        printf("\n");    
    }

    return 0;
}

void mat_mul(int **A, int **B, int r, int x, int c, int **C) {
    int i, j;
    pthread_t *threads = (pthread_t*)malloc(r*c*sizeof(pthread_t));
    data d;
    arg **a = (arg**)malloc(r*sizeof(arg*));

    for(i = 0; i < r; i++)
        a[i] = (arg*)malloc(c*sizeof(arg));

    d.A = A;
    d.B = B;
    d.x = x;
    
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            a[i][j].d = &d;
            a[i][j].i = i;
            a[i][j].j = j;
            a[i][j].result = 0;
            pthread_create(&threads[i], NULL, calc, (void*)&a[i][j]); 
        }
    }

    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            pthread_join(threads[i], NULL);
            C[i][j] = a[i][j].result;
        }
    }
}

void *calc(void* args) {
    arg *a = (arg*)args;
    data *d = a->d;
    int k;

    for(k = 0; k < d->x; k++)
        a->result += (d->A[a->i][k])*(d->B[k][a->j]);

    pthread_exit(NULL);
}