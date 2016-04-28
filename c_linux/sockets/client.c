#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
// проверка на удачное выполнение
#define C(x)                                                                   \
  if (x == -1) {                                                               \
    printf("Error in %s %d, code: %d\n%s\n", __FUNCTION__, __LINE__, errno,    \
           strerror(errno));                                                   \
    exit(1);                                                                   \
  }

#define PORT 8888
#define BUFLEN 512
#define SERVER "127.0.0.1"

int main(void) {
  struct sockaddr_in si_other;
  int s;
  socklen_t slen = sizeof(si_other);
  char buf[BUFLEN];
  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  C(s);
  int enable = 1;
  C(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)));

  memset((char *)&si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  C(inet_aton(SERVER, &si_other.sin_addr));

  memset(buf, 0, sizeof(char) * BUFLEN);
  printf("REQUEST:\n%d\n", getpid());
  fflush(stdout);

  sprintf(buf, "%d\n", getpid());
  C(sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&si_other, slen));
  memset(buf, 0, sizeof(char) * BUFLEN);

  int recvlen =
      recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen);
  C(recvlen);
  printf("SOURCE:\n%s:%d\n", inet_ntoa(si_other.sin_addr),
         ntohs(si_other.sin_port));
  printf("CONTENT:\n%s\n", buf);
  // if (sscanf(buf, "%d\n", &s) != 1)
  //{

  /*  continue;
  }
  float avg = 1488.0f;
  printf( "AVERAGE : %f SEED : %d\n", avg, seed);

  char* replybuf = (char*)malloc(BUFLEN * sizeof(char));
  sprintf(replybuf,"All ok\n");
  C(sendto(s,"OK", 3, 0, (struct sockaddr*)&si_other, slen ));
  */
  close(0);
  return 0;
}
