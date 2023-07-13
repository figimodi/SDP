#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdin.h>
#include <stdlib.h>

#define MAX_SIZE 1024

int main(int argc, char const *argv[])
{
    int in1, in2, out;
    int size1, size2, size;
    void *in1_mapped, *in2_mapped, *out_mapped;
    int i = 0;
    int *p1, *p2;

    in1 = open(argv[1], O_RDONLY);
    in2 = open(argv[2], O_RDONLY);
    out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    if (in1 < 0 || in2 < 0 || out  < 0)
    {
        printf("Error while opening the files passed as arguments\n");
        exit(EXIT_FAILURE);
    }

    read(in1, &size1, sizeof(int));
    read(in2, &size2, sizeof(int));

    size = size1 + size2;
    write(out, &size, sizeof(int));

    in1_mapped = mmap(0, size1*sizeof(float), PROT_READ, MAP_SHARED, in1, sizeof(int));
    in2_mapped = mmap(0, size2*sizeof(float), PROT_READ, MAP_SHARED, in2, sizeof(int));
    out_mapped = mmap(0, size*sizeof(float), PROT_WRITE, MAP_SHARED, out, sizeof(int));

    if (in1_mapped == MAP_FAILED || in2_mapped == MAP_FAILED || out_mapped == MAP_FAILED) 
    {
        printf("Error while mapping the files\n");
        exit(EXIT_FAILURE);
    }

    p1 = in1_mapped + size1;
    p2 = in2_mapped + size2;

    while (i < size) {
        if (p1 < in1_mapped) {
            // in1 finished
            while (p2 >= in2_mapped)
                *(out_mapped++) = *(p2--);

            break;
        }

        if (p2 < in2_mapped) {
            // in2 finished
            while (p1 >= in1_mapped)
                *(out_mapped++) = *(p1--);

            break;
        }

        if (*p1 > *p2)
            *(out_mapped++) = *(p1--);
        else
            *(out_mapped++) = *(p2--);
        
        i++;
    }

    munmap(in1_mapped, size1);
    munmap(in2_mapped, size2);
    munmap(out_mapped, size);

    close(in1);
    close(in2);
    close(out);

    return 0;
}
