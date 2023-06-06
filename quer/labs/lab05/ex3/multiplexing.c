#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#define WAIT_TIME_1 5
#define WAIT_TIME_2 1
#define STR_SIZE 5
#define STR_NUM 4

#define A_LOWCASE 97
#define ALPHABET_LENGTH 26

// int select (
//     int maxfdp1,
//     fd_set *restrict readfds,
//     fd_set *restrict writefds,
//     fd_set *restrict exceptfds,
//     struct timeval *restrict tvptr
// );

// #include <sys/select.h>
// void FD_SET (int fd, fd_set *fdset);
// void FD_CLR (int fd, fd_set *fdset);
// void FD_ZERO (fd_set *fdset);
// int FD_ISSET (int fd, fd_set *fdset);

// fd_set rset;
// int fd;
// FD_ZERO (&rset);
// FD_SET (fd, &rset);
// FD_SET (STDIN_FILENO, &rset)
// if (FD_ISSET(fd, &rset)) { ... }

// fd_set set;
// FD_ZERO (&set);
// FD_SET (fd1, &set);
// FD_SET (fd2, &set);
// FD_SET (fd3, &set);
// size = max(fd1, fd2, fd3) + 1;
// while (1) {
//     // Block until at least ONE input arrives
//     if (select (size, &set, NULL, NULL, NULL) < 0) {
//         // handle error
//     }
//     for (i=0; i<size; ++i) {
//         if (FD_ISSET (i, &set)) {
//         // there is data for fd in position i
//         }
//     }
// }

void Reader(int pipe1[2], int pipe2[2]) {
    fd_set my_set;
    FD_ZERO(&my_set);
    FD_SET(pipe1[0], &my_set);
    FD_SET(pipe2[0], &my_set);
    int size = (pipe1[0] > pipe2[0] ? pipe1[0] : pipe2[0]) + 1;

    close(pipe1[1]);
    close(pipe2[1]);

    int i, j;
    int counter = 0;
    char message[STR_SIZE];

    while(1) {
        if (select(size, &my_set, NULL, NULL, NULL) < 0)
        {
            printf("Error while reading with file descriptors %d or %d\n", pipe1[0], pipe2[0]);
            exit(EXIT_FAILURE);
        }

        for (j = 0; j < size; j++) {
            if (FD_ISSET(j, &my_set)) {
                int n_byte = read(j, message, sizeof(message));
                message[n_byte] = '\0';

                for(i = 0; i < STR_SIZE && message[i] != '\0'; i++) {
                    message[i] -= 32;
                }

                printf("read %d bytes from fd%d: %s\n", n_byte, j, message);
                counter++;
            }
        }

        if (counter == STR_NUM * 2)
            break;
    }

    close(pipe1[0]);
    close(pipe2[0]);
}

void Writer(int id, int pipe[2]) {
    srand(getpid());

    close(pipe[0]);

    char message[STR_SIZE];
    int i, j;
    int n;

    for (i = 0; i < STR_NUM; i++) {
        switch (id)
        {
        case 1:
            sleep(WAIT_TIME_1);
            break;

        case 2:
            sleep(WAIT_TIME_2);
            break;
        default:
            break;
        }

        n = (rand() % (STR_SIZE - 1)) + 1;

        for (j = 0; j < n; j++) {
            int c = rand() % ALPHABET_LENGTH + A_LOWCASE;
            message[j] = c;
        }   
        message[n] = '\0';

        printf("Writer %d => Reader: %s\n", id, message);
        write(pipe[1], message, n);
    }

    close(pipe[1]);
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
        Writer(1, pipe1);
    }
    else if (p1id > 0){
        p2id = fork();
        if (p2id == 0) {
            Writer(2, pipe2);
        }
        else if (p2id > 0){
            Reader(pipe1, pipe2);
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

// Writer 2 => Reader: ub
// read 2 bytes from fd5: UB
// Writer 2 => Reader: jppa
// read 4 bytes from fd5: JPPA
// Writer 2 => Reader: odob
// read 4 bytes from fd5: ODOB
// Writer 2 => Reader: bk
// read 2 bytes from fd5: BK
// Writer 1 => Reader: gy
// Writer 1 => Reader: n
// Writer 1 => Reader: l
// Writer 1 => Reader: ale