#include <stdio.h> //stdout, stderr
#include <pthread.h> //threads
#include <string.h> //memcpy
#include <stdlib.h> //malloc
#include <semaphore.h> //semaphore

typedef struct {
    pthread_t *path;
    int n;
    int size;
} thread_arg;

sem_t mutex;

void *thread_generation(void* arg);

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    thread_arg t_arg;
    t_arg.n = n;
    t_arg.size = n;
    t_arg.path = (pthread_t*)malloc(n*sizeof(pthread_t));
    sem_init(&mutex, 0, 1);

    if(t_arg.path == NULL) {
        fprintf(stderr, "Error while allocating, memory full!");
        exit(1);
    }

    thread_generation((void *)(&t_arg));

    return 0;
}

void *thread_generation(void* arg) {
    /*
        if n>0
            mark down pthread_self in path in position path.length - n
            make a copy of thread_arg
            create two threads
            join the threads
        else
            print path
        exit thread
    */
    int i;
    thread_arg *og = (thread_arg*)arg;
    
    if(og->n > 0) {
        thread_arg *cpy = (thread_arg*)malloc(sizeof(thread_arg));
        pthread_t *path_cpy = (pthread_t*)malloc(og->size*sizeof(pthread_t));
        pthread_t first, second;

        if(cpy == NULL) {
            fprintf(stderr, "Error while allocating, memory full!");
            exit(1);
        }

        og->path[og->size - og->n] = pthread_self();
        og->n = og->n - 1;
        memcpy(cpy, og, sizeof(thread_arg));
        memcpy(path_cpy, og->path, og->size*sizeof(pthread_t));
        cpy->path = path_cpy;
        
        pthread_create(&first, NULL, thread_generation, og);
        pthread_create(&second, NULL, thread_generation, cpy);

        pthread_join(first, NULL);
        pthread_join(second, NULL);
    }
    else {
        sem_wait(&mutex);
        for(i = 0; i < og->size; i++) {
            fprintf(stdout, "%li ", og->path[i]);
        }
        fprintf(stdout, "\n");
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}
