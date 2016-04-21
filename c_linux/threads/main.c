#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TNUM 4

pthread_mutex_t mutex;

// проверка на удачное выполнение
#define C(x)                                                                   \
  if (x != 0) {                                                                \
    printf("Fail in %s %d\n", __FUNCTION__, __LINE__);                         \
    exit(EXIT_FAILURE);                                                        \
  }

struct PARAMS {
  float *start;
  size_t size;
};

/*
        Находит среднее по масссиву a размерности n
*/
float avg(float *a, int n) {
  float sum = 0.0f;
  for (int i = 0; i < n; i++) {
    sum += a[i];
  }
  float avg = sum / (float)n;
  return avg;
}
/*
      Работа для потоков, вход - структура PARAMS
*/
static void *thread_func(void *arg) {
  struct PARAMS *params = (struct PARAMS *)arg;

  C(pthread_mutex_lock(&mutex));
  // чтение из общих данных
  float avgf = avg(params->start, params->size);
  // запись
  for (size_t i = 0; i < params->size; i++) {
    params->start[i] += rand() % 1000;
  }
  C(pthread_mutex_unlock(&mutex));

  printf("Thread : avg =  %f\n", avgf);
  return NULL;
}

int main(void) {
  pthread_t thread[TNUM];

  C(pthread_mutex_init(&mutex, 0));
  size_t n = rand() % 100000 + 100;
  float *ptr = (float *)malloc(sizeof(float) * n);

  for (size_t i = 0; i < n; i++) {
    ptr[i] = rand() % 1000;
  }

  struct PARAMS params;
  params.start = ptr;
  params.size = n;
  printf("start = %p\nsize = %zu\n", params.start, params.size);

  for (int i = 0; i < TNUM; i++) {
    C(pthread_create(&thread[i], NULL, thread_func, &params));
  }
  for (int i = 0; i < TNUM; i++)
    C(pthread_join(thread[i], NULL));

  C(pthread_mutex_destroy(&mutex));
  printf("Stop working\n");

  return EXIT_SUCCESS;
}
