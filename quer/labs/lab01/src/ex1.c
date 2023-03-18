#include <stdio.h>
#include <fcntl.h>
#define MAX_LENGTH 31

typedef struct record {
    int id;
    long registerNumber;
    char surname[MAX_LENGTH];
    char name[MAX_LENGTH];
    int mark;
} record_t;

int main(int argc, char **argv) {

    // argc[1]: file_1
    // argc[2]: file_2
    // argc[3]: file_3
    FILE *fp1, *fp3;
    int fd2, not_end;
    record_t recTo, recFrom;

    // copy from 1 to 2
    fp1 = fopen(argv[1], "r");
    fd2 = open(argv[2], O_WRONLY | O_CREAT); 
    while (fscanf(fp1, "%d %ld %s %s %d", 
                &(recTo.id),
                &(recTo.registerNumber),
                recTo.surname,
                recTo.name,
                &(recTo.mark)) != EOF) {

        write(fd2, &recTo, sizeof(record_t));
    }
    fclose(fp1);
    close(fd2);


    // copy from 2 to 3
    fd2 = open(argv[2], O_RDONLY);
    fp3 = fopen(argv[3], "w");

    do {
        not_end = read(fd2, &recFrom, sizeof(record_t));
        if (not_end) {
            fprintf(fp3, "%d %ld %s %s %d\n", 
                    recFrom.id,
                    recFrom.registerNumber,
                    recFrom.surname,
                    recFrom.name,
                    recFrom.mark);
        }
    } while (not_end);
    close(fd2);
    fclose(fp3);

}

