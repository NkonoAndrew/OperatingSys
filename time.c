#include <fcntl.h> // O_CREAT, O_RDWR
#include <stdio.h> // printf(), stderr
#include <unistd.h> // ftruncate(), fork(), execvp()
#include <sys/mman.h> // shm_open(), mmap(), PROT_READ, PROT_WRITE, MAP_SHARED, shm_unlink()
#include <sys/shm.h>
#include <sys/time.h> // timeval, gettimeofday(), timersub()
#include <sys/wait.h> // wait()

#define BUFFER SIZE 25
#define READ END 0
#define WRITE END 1

int main(int argc, char ** argv)
{
    
    int fd[2];
    int sharedmemfd; //shared memory file descriptor
    pid_t pid;
    typedef struct timeval timeval_t;
    
    /*SIZE of shared memory */
    const int SIZE = 4096;
    
    /* name of the shared memory object*/
    const char *name = "OS";
    
    
    /* create the pipe */
    if (pipe(fd) == -1) {
       fprintf(stderr,"Pipe failed");
    return 1;
    }
    
    /* establish region of shared memory */
    sharedmemfd = shm_open(name, O_CREAT | O_RDWR, 0666);
    
    //configure the size of the shared memory
    ftruncate(sharedmemfd, SIZE);
    
    // memory map shared memory object */
    timeval_t *sharedMemory = mmap (0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, sharedmemfd, 0);
        
        
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */ fprintf(stderr, "Fork Failed");
        return 1;
    }
    if (pid > 0) { /* parent process */
        wait(NULL);
        
        //get the end time
        timeval_t endTime;

        gettimeofday(&endTime, NULL);
        
        //get the start time from the shared memory
        timeval_t startTime;
        startTime = *sharedMemory;
        
        /* remove the shared memory object */
        shm_unlink(name);
        
        /*find the elapsed time*/
        timeval_t elapsedTime;
        timersub( &endTime, &startTime, &elapsedTime);
        
        // print microseconds right justified zero filled
        printf( "\nElapsed time: %ld.%06d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec );
       
    }
    else { /* child process */
        
        //get the time and save it in shared memory
        gettimeofday(sharedMemory, NULL);
        
        execvp( argv[1], argv + 1);

    }
    return 0;

}
