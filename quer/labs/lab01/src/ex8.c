#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int last_signal = -1;
int last_last_signal = -1;
int finish = 0;

void sign_handler(int signal) 
{
    //check that is not the first signal received, if it's already the second one then go into the if
    if(last_signal != -1)
    {
        if(last_signal != signal)
            printf("success\n");
        else
            fprintf(stderr, "error\n");
    }

    //check if three consecutive signals were the same
    if(last_last_signal == last_signal && last_signal == signal)
        finish = 1;

    //update the history of the signals
    last_last_signal = last_signal;
    last_signal = signal;
}

int main(void) {
    if((signal(SIGUSR1, sign_handler) == SIG_ERR) || (signal(SIGUSR2, sign_handler) == SIG_ERR))
        printf("Error initializing singals\n");
    
    //wait untill the sign handler change finish into 1
    while(!finish);
}