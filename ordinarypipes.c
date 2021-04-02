#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(){

 char write_msg[BUFFER_SIZE] = "HI THERE";
 char read_msg[BUFFER_SIZE];
 int pipeOne[2];
 int pipeTwo[2];
 pid_t pid;

/* create a pipe */
 if (pipe(pipeOne) == -1) {
   return 1;
 }

/* create pipe two */
 if (pipe(pipeTwo) == -1 ) {
   return 1;
 }


 /* fork a child process */
 pid = fork();
 
 if (pid < 0) {
  return 1;
 }

 if (pid > 0){ /* parent process */
  /* close unused ends of the pipes */
  close(pipeOne[READ_END]);
  close(pipeTwo[WRITE_END]);



  /* write to pipe */
   write(pipeOne[WRITE_END], write_msg, strlen( write_msg) + 1);

  /* close the write end of pipe one */
   close(pipeOne[WRITE_END]);

 
   
   /* read  end of pipe two */
   read(pipeTwo[READ_END] , read_msg, BUFFER_SIZE);
   printf("The child sent %s\n", read_msg);   

 
   /*close pipe two read end */
   close(pipeTwo[READ_END]);

 }
 else { /* child process */
   /* close unused ends of the pipe */
   close(pipeOne[WRITE_END]);
   close(pipeTwo[READ_END]);  


   /* read from the pipe */
   read(pipeOne[READ_END],  read_msg, BUFFER_SIZE);
   printf("The parent sent %s\n", read_msg);

   /* close the read end of pipe one */
   close(pipeOne[READ_END]);

  
   char * message = read_msg ;
   
   int i = 0;

   while (message[i] != '\0') {
      if (message[i] > 64 && message[i] < 91) {
           message[i] += 32; 
      }
      else if ( message[i] > 96 && message[i] < 123) {
          message[i] -= 32; 
      }
      i++;
   }
    
  
   /* write end of pipe two */
   write(pipeTwo[WRITE_END] , message, strlen(message) + 1);

   /*close pipe two write end */
   close(pipeTwo[WRITE_END]);
 }
  
 return 0;


}
