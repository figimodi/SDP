#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define WAIT_TIME_1 1
#define WAIT_TIME_2 5
#define STR_SIZE 5
#define STR_NUM 4

#define A_LOWCASE 97
#define ALPHABET_LENGTH 26

void set_fnctl(int fd, int flags) {
    int previuous_flags;
    if ((previuous_flags = fcntl(fd, F_GETFL, 0)) < 0)
    {
        printf("Error while reading file descriptor flags\n");
        exit(EXIT_FAILURE);
    }

    previuous_flags |= flags;

    if (fcntl(fd, F_SETFL, previuous_flags) < 0)
    {
        printf("Error while reading file descriptor flags\n");
        exit(EXIT_FAILURE);
    }
}

void P1(int pipe1[2]) {
    set_fnctl(pipe1[1], O_NONBLOCK);

    srand(getpid());

    char message[STR_SIZE];
    int i, j;
    int n;

    for (i = 0; i < STR_NUM; i++) {
        sleep(WAIT_TIME_1);
        n = (rand() % (STR_SIZE - 1)) + 1;

        for (j = 0; j < n; j++) {
            int c = rand() % ALPHABET_LENGTH + A_LOWCASE;
            message[j] = c;
        }   
        message[n] = '\0';

        printf("P1 => P: %s\n", message);
        write(pipe1[1], message, n);
    }

    close(pipe1[1]);
}

void P2(int pipe2[2]) {
    set_fnctl(pipe2[1], O_NONBLOCK);

    srand(getpid());

    char message[STR_SIZE];
    int i, j;
    int n;

    for (i = 0; i < STR_NUM; i++) {
        sleep(WAIT_TIME_2);
        n = (rand() % (STR_SIZE - 1)) + 1;

        for (j = 0; j < n; j++) {
            int c = rand() % ALPHABET_LENGTH + A_LOWCASE;
            message[j] = c;
        }   
        message[n] = '\0';

        printf("P2 => P: %s\n", message);
        write(pipe2[1], message, n);
    }

    close(pipe2[1]);
}

void P(int pipe1[2], int pipe2[2]) {
    set_fnctl(pipe1[0], O_NONBLOCK);
    set_fnctl(pipe2[0], O_NONBLOCK);

    int i;
    int counter = 0;
    char message[STR_SIZE];

    while(1) {
        int n_pipe1 = read(pipe1[0], message, sizeof(message));
        if( n_pipe1 > 0) {
            message[n_pipe1] = '\0';
            for(i = 0; i < STR_SIZE && message[i] != '\0'; i++) {
                message[i] -= 32;
            }
            printf("read %d bytes from P1: %s\n", n_pipe1, message);
            counter++;
        }

        int n_pipe2 = read(pipe2[0], message, sizeof(message));
        if( n_pipe2 > 0) {
            message[n_pipe2] = '\0';
            for(i = 0; i < STR_SIZE && message[i] != '\0'; i++) {
                message[i] -= 32;
            }
            printf("read %d bytes from P2: %s\n", n_pipe2, message);
            counter++;
        }

        if (counter == STR_NUM * 2)
            break;
    }

    close(pipe1[0]);
    close(pipe2[0]);
}

int main(int argc, char const *argv[])
{
    int pipe1[2], pipe2[2];
    int p1id, p2id;

    if (pipe(pipe1)) {
        printf("Error while creating the pipe\n");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipe2)) {
        printf("Error while creating the pipe\n");
        exit(EXIT_FAILURE);
    }

    p1id = fork();
    if (p1id == 0) {
        P1(pipe1);
    }
    else if (p1id > 0){
        p2id = fork();
        if (p2id == 0) {
            P2(pipe2);
        }
        else if (p2id > 0){
            P(pipe1, pipe2);
        }
        else {
            printf("Error while forking\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        printf("Error while forking\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
