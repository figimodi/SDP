#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_LENGTH 10
#define ALPHABET_LENGTH 26
#define A 97

void P1(int fd1[2], int fd2[2]) {
    close(fd1[0]); 
    close(fd2[1]);

    srand(getpid());

    char message[MAX_LENGTH];
    int i;
    int n;

    while (1)
    {
        n = rand() % MAX_LENGTH;

        for(i = 0; i < n; i++)
        {
            int c = rand() % ALPHABET_LENGTH + A;
            message[i] = c;
        }
        message[n] = '\0';

        printf("p1 => p2: %s\n", message);
        write(fd1[1], message, n);
        
        if(n == 0)
            break;
        
        int n_read = read(fd2[0], message, sizeof(message));

        if(n_read == 0)
        {
            printf("end of transmission\n");
            break;
        }

        message[n_read] = '\0';

        for(i = 0; i < MAX_LENGTH && message[i] != '\0'; i++) {
            message[i] -= 32;
        }

        printf("p1 recived %d characters: %s\n", n_read, message);
    }

    close (fd1[1]);
    close (fd2[0]);
    return;
}

void P2(int fd1[2], int fd2[2]) {
    close(fd1[1]);
    close(fd2[0]);

    srand(getpid());

    char message[MAX_LENGTH];
    int i;
    int n;

    do {
        int n_read = read(fd1[0], message, sizeof(message));


        if(n_read == 0)
        {
            printf("end of transmission\n");
            break;
        }

        message[n_read] = '\0';

        for(i = 0; i < MAX_LENGTH && message[i] != '\0'; i++) {
            message[i] -= 32;
        }

        printf("p2 recived %d characters: %s\n", n_read, message);
        n = rand() % MAX_LENGTH;

        for(i = 0; i < n; i++)
        {
            int c = rand() % ALPHABET_LENGTH + A;
            message[i] = c;
        }
        message[n] = '\0';

        printf("p2 => p1: %s\n", message);
        write(fd2[1], message, n);

        if(n == 0)
            break;
    }
    while (1);

    close (fd1[0]);
    close (fd2[1]);
    return;
}

int main(int argc, char const *argv[])
{
    int fd1[2], fd2[2];
    int p2id;

    if (pipe(fd1)) {
        printf("Error while creating the pipe\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2)) {
        printf("Error while creating the pipe\n");
        exit(EXIT_FAILURE);
    }

    p2id = fork();
    if(p2id == 0) {
        P2(fd1, fd2);
    }
    else if (p2id > 0){
        P1(fd1, fd2);
    }
    else {
        printf("Error while forking\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

