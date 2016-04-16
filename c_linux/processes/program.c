#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 


#define PNUM 10

void start_process()
{
  if (exec("./work","work",NULL)<0) {
   printf("ERROR while start process\n");
   exit(-2);
}
}

int main()
{
    printf("Parent: Starting %d processes...\n", PNUM);
    
    for(int i  = 0; i < PNUM; i++)
    {
      pid_t p  = fork();
      if (p == 0) start_process(); //work();
    }
    printf("I am parent %d\n", getpid());
    //p = fork();
    //if (p == 0) start_process();//work();
    
    for (int i = 0; i < PNUM; i++)
    {
      int status = 0;
      pid_t child = wait(&status);
      if (child > 0)
        printf("Parent: Child %d worked\n", (int)child);
    }
    printf("Parent: All processes worked\n");
    
    /*child = wait(&status);
    if (child > 0)
      printf("Parent: Child %d worked\n", (int)child);
    else printf("Parent: All processes already worked\n");*/
    return 0;
}
