/*
	PIPE EXAMPLE
	============
*/

#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "sys/types.h"

/*
	Максимальный размер строки
*/
#define SIZE 32
/*
	Число процессов 
*/
#define P_NUM 10

/*
	Находит среднее по масссиву a размерности n
*/
float avg(float* a, int n) {
	float sum = 0.0f;
	for (int i = 0; i < n; i++) {
		sum += a[i];
	}
	float avg = sum / (float)n;
	return avg;
}
/*
	Генерирует массив случайных чисел случайного размера
	Находит по нему математическое ожидание
	Записывает в файл с дескрипотором fd id процесса и 
	мат. ожидание
*/
void work(int fd) {
	srand(getpid());
	int n = rand() % 1000;
	float* mem = (float*)malloc(sizeof(float) * n);
	for (int i = 0; i < n; i++) mem[i] = rand() % 1000;
	float average = avg(mem, n);
	char buf[SIZE];
	int len = sprintf(buf, "pid = %d\tavg = %f\n", (int)getpid(), average);
	write(fd, buf, len);
	free(mem);
}

int main() {

	int outs[P_NUM];

	/*
		Cоздаем P_NUM процессов
	*/

	for (int i = 0; i < P_NUM; i++)
	{
		int fd[2];
		pipe(fd);

		pid_t p = fork();

		if (p == 0)
		{

			close(fd[0]);
			work(fd[1]);
			return 0;
		}
		else
		{
			close(fd[1]);
			outs[i] = fd[0];
		}
	}

	/*
		Выводим записи дочерних процессов
	*/
	printf("Messages:\n");
	for (int i = 0; i < P_NUM; i++)
	{
		char buf[SIZE];
		int n = read(outs[i], buf, sizeof(buf));
		printf("%s", buf);
	}
	return 0;
}