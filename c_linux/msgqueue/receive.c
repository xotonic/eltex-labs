#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE     128

void die(const char *s)
{
  perror(s);
  exit(1);
}

struct msgbuf
{
    long    mtype;
    char    mtext[MAXSIZE];
};

 int main()
 {
 	int msgid;
 	key_t key = 1488;
 	struct msgbuf rbuf;

 	if ((msgid = msgget(key, 0666)) < 0) die("msgget");
#ifdef WAIT
 	while (msgrcv(msgid, &rbuf, MAXSIZE, 1, 0) >= 0)
 		printf("%s\n", rbuf.mtext);
#else
 	while (msgrcv(msgid, &rbuf, MAXSIZE, 1, IPC_NOWAIT) >= 0)
 			printf("%s\n", rbuf.mtext);
#endif
 	return 0;
 }