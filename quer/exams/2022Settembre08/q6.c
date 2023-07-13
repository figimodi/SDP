#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdin.h>
#include <unistd.h>

#define MAX_SIZE 1024

int stop_fifo = 0;

void P (int fd1, int fd2, void* out_mapped) {
    fd_set read_set;
    FD_ZERO(&read_set);
    FD_SET(fd1, &read_set);
    FD_SET(fd2, &read_set);
    int size = (fd1 > fd2 ? fd1 : fd2) + 1;

    int i, nbyte, max_byte = 0, n_zeros = 0;
    float value;

    while (1) {
        if (select(size, &read_set, NULL, NULL, NULL) < 0)
        {
            printf("error while reading\n");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < size; i++) {
            if (FD_ISSET(i, &read_set)) {
                nbyte = read(i, &value, sizeof(float));
                if (!nbyte)
                    break;

                if (value == 0) {
                    if (i == fd1)
                        close(fd1);
                    else if (i == fd2)
                        close(fd2);
                    n_zeros++;

                    if (n_zeros == 2)
                        return;
                } 

                *(out_mapped++) = value;
                max_byte++;

                if(max_byte >= MAX_SIZE)
                    stop_fifo = 1;
                    close(fd1);
                    close(fd2);
                    return;
            }
        }
    }
}

void Pgen (int fd) {
    srand(getpid());
    int random_int, random_time;
    float random_float;
    
    random_time = rand() % 3000;

    while (!stop_fifo) {
        random_int = rand();
        random_float = float(random_int) / 100;
        sleep(random_time);
        wirte(fd1, random_float, sizeof(float));

        if (random_float == 0)
        {
            close(fd);
            break;
        }
    }
} 

int main(int argc, char const *argv[])
{
    int fd1_write, fd1_read, fd2_write, fd2_write, out;
    void* out_mapped;

    mkfifo("/temp/fifo1", 0666);
    mkfifo("/temp/fifo2", 0666);

    fd1_write = open("/temp/fifo1", O_WRONLY);
    fd1_read = open("/temp/fifo1", O_RDONLY | O_NONBLOCK);
    fd2_write = open("/temp/fifo2", O_WRONLY);
    fd2_read = open("/temp/fifo2", O_RDONLY | O_NONBLOCK);

    out = open("out", O_WRONLY);
    out_mapped = mmap(0, MAX_SIZE, PROT_WRITE, MAP_SHARED, out, 0);

    if (fork()) {
        if (fork()) {
            P();
            munmap(out_mapped, MAX_SIZE);
        }
        else
            Pgen();
    }
    else
        Pgen();

    return 0;
}
