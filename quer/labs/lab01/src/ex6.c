#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_LENGTH 5

int main(int argc, char*argv[]) {

    //argv[1] = h and argv[2] = n
    int h = atoi(argv[1]);
    int n = atoi(argv[2]);
    int i = 0; 
    int pid = -1;
    char h_str[MAX_LENGTH];

    if(h == 0)
    {
        printf("%d\n", getpid()); //print the pid of the current process
        return 1;
    }


    for(i = 0; i < n; i++)
    {
        pid = fork();
        // if pid == 0 => we're in the child, if pid != 0 then we're in the parent
        if(!pid) {
            //child
            sprintf(h_str, "%d", h-1); //h_str = h-1
            execlp(argv[0], argv[0], h_str, argv[2], (char *)0); // override of the process with this new one with n=n and h=h-1
        }
    }

    return 1;
}