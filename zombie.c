#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
pid_t pid;

    /*fork a child process*/
    pid = fork();

    //parent
    if (pid > 0){
        sleep(10);
        printf("Parent: %d\n", getpid());
    }
    
    else {
        printf("Child: %d\n" , getpid());
    }
    return 0;
   
}
