#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unistd.h"
#define MAXSIZE 128

void die(const char *s)
{
	perror(s);
	exit(1);
}


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
	Записывает в buf с дескрипотором fd id процесса и
	мат. ожидание
	Возвращает длину строки
*/
int work(char* buf) {
	srand(getpid());
	int n = rand() % 1000;
	float* mem = (float*)malloc(sizeof(float) * n);
	for (int i = 0; i < n; i++) mem[i] = rand() % 1000;
	float average = avg(mem, n);
	//char buf[MAXSIZE];
	int len = sprintf(buf, "pid = %d\tavg = %f", (int)getpid(), average);
	free(mem);
	return len;
}


struct smgbuf
{
	long mtype;
	char mtext[MAXSIZE];
};

int main()
{
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	struct smgbuf sbuf;
	size_t buflen = 0;
	key_t key = 1488;

	if ( (msqid = msgget(key, msgflg)) < 0 ) die("msgget");

	sbuf.mtype = 1;

	//buflen = snprintf(sbuf.mtext, MAXSIZE, "Message from %d", getpid());
	buflen = work(sbuf.mtext);
	if ( msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) die("msgsnd");
	else printf("Message sent\n");
	return 0;
}