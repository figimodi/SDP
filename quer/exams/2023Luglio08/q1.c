#define SIZE_ARRAY 10

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

void bubble_sort(int buffer[SIZE_ARRAY]) {
    int i, j, temp;

    for (i = 0; i < SIZE_ARRAY; i++) {
        for (j = 0; j < SIZE_ARRAY - i - 1; j++) {
            if (buffer[j] > buffer[j + 1]) {
                temp = buffer[j + 1];
                buffer[j + 1] = buffer[j];
                buffer[j] = temp;
            }
        }
    }
}

void P1(int fd1[2], int fd2[2]) {
    //writes on fd1
    //reads on fd2
    close(fd1[0]);
    close(fd2[1]);
    
    int buffer[SIZE_ARRAY];
    int i, x;
    srand(getpid());

    for (x = 0; x < 3; x++) {
        // generate random values into array
        printf("P1: array generated by p1: ");
        for (i = 0; i < SIZE_ARRAY; i++)
        {
            buffer[i] = rand() % 40;
            printf("%d ", buffer[i]);
        }
        printf("\n");

        // write f1
        write(fd1[1], buffer, SIZE_ARRAY*sizeof(int));
        printf("P1: array sent to p2 \n");

        // read from fd2
        read(fd2[0], buffer, SIZE_ARRAY*sizeof(int));
        printf("P1: read array from p2\n");

        // print 
        printf("P1: array returned by p2: ");
        for (i = 0; i < SIZE_ARRAY; i++)
            printf("%d ", buffer[i]);
        printf("\n");
    }
}

void P2(int fd1[2], int fd2[2]) {
    //writes on fd2
    //reads on fd1
    close(fd1[1]);
    close(fd2[0]);

    int x, i;

    int buffer[] = { 0, 5, 2, 3, 4, 1, 9, 8, 7, 6};

    for(x = 0 ; x < 3; x++){
        // sort array
        bubble_sort(buffer);
        printf("P2: array ordered: \n");
        for (i = 0; i < SIZE_ARRAY; i++)
        printf("%d ", buffer[i]);
        printf("\n");

        // write fd2
        write(fd2[1], buffer, SIZE_ARRAY*sizeof(int));
        printf("P2: array ordered sent\n");

        // read fd1 
        read(fd1[0], buffer, SIZE_ARRAY*sizeof(int));
        printf("P2: received array from p1:\n");
        for (i = 0; i < SIZE_ARRAY; i++)
        printf("%d ", buffer[i]);
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    int fd1[2], fd2[2];

    if (pipe(fd1)) {
        printf("error while initializing pipeline 1");
        exit(EXIT_FAILURE);
    }

    if (pipe(fd2)) {
        printf("error while initializing pipeline 2");
        exit(EXIT_FAILURE);
    }

    if (fork()) 
        P1(fd1, fd2);
    else
        P2(fd1, fd2);

    return 0;
}
