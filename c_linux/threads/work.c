#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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

int main()
{
  srand(time(0));
  int n = rand()%1000;
  float* mem = (float*)malloc(sizeof(float)* n);
  for (int i = 0; i < n; i++)
      mem[i] = rand()%1000;
  float average = avg(mem, n);
  printf("Child: pid = %d\tavg = %f\n", (int)getpid(), average);
  free(mem);
  return 0;
}