#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void binary(int* vett, int n, int i);

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int *vett = (int*)malloc(n*sizeof(int));
    int i = 0;

    if (vett == NULL) {
        fprintf(stderr, "Error while allocating\n");
        exit(1);
    }

    printf("Binary numbers:\n");
    binary(vett, n, i);
    free(vett);
}

void binary(int* vett, int n, int i) {
    for (i = 0; i < n; i++) {
        if (fork()) {
            //parent
            wait((int *)0); // the parent wait for the children since it can't modify the vett while they're working on it
            vett[i] = 1;
        }            
        else {      
            //child 
            vett[i] = 0;
        }
    }

    // once i == n we're ready to print the entire vet of numbers
    for (i = 0; i < n; i++)
        printf("%d", vett[i]);
    printf("\n");
    return;
}
