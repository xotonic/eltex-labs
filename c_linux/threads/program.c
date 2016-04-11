#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 


void start_process()
{
  if (execl("./work","work",NULL)<0) {
   printf("ERROR while start process\n");
   exit(-2);
}
}

int main()
{
    pid_t p  = fork();
    
    if (p == 0) start_process(); //work();
    else 
    {
      p = fork();
      if (p == 0) start_process();//work();
        else 
        {
          int status = 0;
          pid_t child = wait(&status);
          if (child > 0)
            printf("Parent: Child %d worked\n", (int)child);
          else printf("Parent: All processes already worked\n");
          
          child = wait(&status);
          if (child > 0)
            printf("Parent: Child %d worked\n", (int)child);
          else printf("Parent: All processes already worked\n");
        }
    }
    return 0;
}
