#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

#define READ_END 0
#define WRITE_END 1


int main(int argc,char *argv[]) {
//Pipe setup
    int fd[2];
    pid_t pid;
    struct timeval startTime, endTime;
    
    /* create the pipe */
    if (pipe(fd) == -1) {
       fprintf(stderr,"Pipe failed");
    return 1;
    }
    /* fork a child process */
      pid = fork();
    
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed"); return 1;
    }
    
    if (pid > 0) { /* parent process */
        
        wait(NULL);

        double timeTaken;
        long startusec;
        
        close(fd[WRITE_END]);
        
        char buffer[100];
        
        /* read from pipe */
        dup2(fd[READ_END], 0);
        
        scanf( "%ld" , &startusec);
        
        gettimeofday(&endTime, NULL);
        
        timeTaken = (endTime.tv_usec - startusec);

        printf("Elapsed time: %0.6lf ",timeTaken);

        close(fd[READ_END]);

        
            

    }
    else { /* child process */
    /* close the unused end of the pipe */
        close(fd[READ_END]);
        
        gettimeofday(&startTime, NULL);

        char buffer[100];
        
        sprintf(buffer, "%d", startTime.tv_usec);
                
    /* write to the pipe */
        write(fd[WRITE_END], buffer, strlen(buffer) + 1);
        
        
        execvp(argv[1], argv + 1);
        
    /* close the read end of the pipe */
        close(fd[WRITE_END]);
        
        
    }
    return 0;
    
}
