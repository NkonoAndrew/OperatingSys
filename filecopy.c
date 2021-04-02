#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h> 

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]){

    int fileD[2];  /* FILE  file descriptors */
    int pipeD[2];/* PIPE file descriptors */
    pid_t pid;
    char buffer[2];
    int numBytes;


    fileD[0] = open(argv[1], O_RDONLY);
    fileD[1] = open(argv[2], O_CREAT | O_RDWR);

/* create a pipe */ 
	if (pipe(pipeD) == -1){
	   return 1;
	} 
/* fork a child process */
	pid = fork();

	if (pid < 0){
          return 1;
    }

    else if (pid > 0){ /* parent process */
 	/* close read end of the pipe */
    close(pipeD[0]);
            
    /* reads a byte at a time from file and write to pipe */
        while ((numBytes = read(fileD[0], buffer, 1) > 0)) {
            write(pipeD[1], buffer, numBytes);
        }
        
    /*close the write end of the pipe*/
        close(pipeD[1]);
        
    /* close the file */
        close(fileD[0]);
	}

    else {
        
        /* close write end of the pipe */
        close(pipeD[1]);
        
        /* read from the read end of the pipe and writes to the file */
        while ((numBytes = read(pipeD[0], buffer, 1) > 0)) {
            write(fileD[1], buffer, numBytes);
        }
        
    /* close the read end of the pipe */
        close(pipeD[0]);
        
    /* close the file */
        close(fileD[1]);
        
    }

}
