#include <stdio.h>
#include <fcntl.h>
#define MAX_LENGTH 31

void readFrom(int fd, int id);
void writeIn(int fd, int id);
void menu(char* fileName);

typedef struct record {
    int id;
    long registerNumber;
    char surname[MAX_LENGTH];
    char name[MAX_LENGTH];
    int mark;
} record_t;

int main(int argc, char **argv) {
    
    // argv[1]: input file

    menu(argv[1]);

    return 0;
}

void menu(char* fileName) {
    char opt;
    int n, fd;
    int end = 0;
    
    fd = open(fileName, O_RDWR);

    printf("Choose an option from:\n[R n]\n[W n]\n[E]\n");
    
    while(!end) {            
        printf("Write the option: ");
        scanf("%c", &opt);
        switch (opt)
        {
            case 'R':
                scanf("%d%*c", &n);
                readFrom(fd, n);
                break;
            case 'W':
                scanf("%d%*c", &n);
                writeIn(fd, n);
                break;
            case 'E':
                end = 1;
                break;
        }
    }
    return;
}

void readFrom(int fd, int n) {
    record_t rec;

    lseek(fd, (n - 1)*sizeof(record_t), SEEK_SET);
    read(fd, &rec, sizeof(record_t));

    printf("%d %ld %s %s %d\n",
            rec.id,
            rec.registerNumber,
            rec.surname,
            rec.name,
            rec.mark);

    return;
}

void writeIn(int fd, int n) {
    record_t rec;

    printf("Write record: ");
    scanf("%ld %s %s %d%*c", &(rec.registerNumber), &(rec.surname), &(rec.name), &(rec.mark));
    rec.id = n;

    lseek(fd, (n - 1)*sizeof(record_t), SEEK_SET);
    write(fd, &rec, sizeof(record_t));

    return;
}
