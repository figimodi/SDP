#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct params{
    int n;
    int id;
} params_t;

int* vector;
int reset;

pthread_barrier_t barrier1;
pthread_barrier_t barrier2;
pthread_cond_t cv_reset;
pthread_mutex_t mutex_reset;

void* tf(void* args) {
    int i;
    params_t* params = ((params_t*)args);
    int n = params->n;
    int id = params->id;

    for (i = 0; i < n; i++) {
        int to_sum;
        int index = id - (1 << i);

        if (index < 0)
            pthread_exit((void*)0);

        pthread_mutex_lock(&mutex_reset);
        while (reset == 0)
            pthread_cond_wait(&cv_reset, &mutex_reset);
        pthread_mutex_unlock(&mutex_reset);

        to_sum = vector[index];
        pthread_barrier_wait(&barrier);

        pthread_mutex_lock(&mutex_reset);
        if (reset == 1) 
            reset = 0;
        pthread_mutex_unlock(&mutex_reset);

        vector[id] += to_sum;
        pthread_barrier_wait(&barrier);
    }

    pthread_exit((void*)0);
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        exit(EXIT_FAILURE);

    srand(time(NULL));

    int i;
    int n = atoi(argv[1]);
    int length = 1 << n;
    
    reset = 1;
    vector = (int*)malloc(length*sizeof(int));
    pthread_t* threads = (pthread_t*)malloc((length - 1)*sizeof(pthread_t));
    params_t* params = (params_t*)malloc(sizeof(length - 1)*sizeof(params_t));

    pthread_barrier_init(&barrier1, NULL, length - 1);
    pthread_barrier_init(&barrier2, NULL, length - 1);
    pthread_mutex_init(&mutex_reset, NULL);

    for (i = 0; i < length; i++)
        vector[i] = rand() % 10;

    for (i = 0; i < length - 1; i++)
    {
        params[i].id = i + 1;
        params[i].n = n;
        pthread_create(&threads[i], NULL, tf, (void*)&params[i]);
    }

    printf("result: %d ", vector[0]);

    for (i = 0; i < length - 1; i++) 
    {
        pthread_join(threads[i], (void **)0);
        printf("%d ", vector[i + 1]);
        pthread_barrier_init(&barrier1, NULL, length - 1 - (i + 1));
        pthread_barrier_init(&barrier2, NULL, length - 1 - (i + 1));
        pthread_mutex_lock(&mutex_reset);
        reset = 1;
        pthread_mutex_unlock(&mutex_reset);
        pthread_cond_broadcast(&cv_reset);
    }

    printf("\n");

    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);
    free(threads);
    free(params);
    free(vector);

    return 0;
}
