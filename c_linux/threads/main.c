#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <time.h>
float avg(float* a, int n)
{
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
    {
        sum += a[i];
    }
    float avg = sum / (float)n;
    return avg;
}

void work()
{
  srand(getpid());
  int n = rand()%1000;
  float* mem = (float*)malloc(sizeof(float)* n);
  for (int i = 0; i < n; i++)
      mem[i] = rand()%1000;
  float average = avg(mem, n);
  printf("Child: pid = %d\tavg = %f\n", (int)getpid(), average);
  free(mem);
}

int main()
{
    pid_t p  = fork();
    
    if (p == 0) work();
    else 
    {
      p = fork();
      if (p == 0) work();
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
