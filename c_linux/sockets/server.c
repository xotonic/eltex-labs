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

float work(int seed)
{
  srand(seed);
  int n = rand()%1000;
  float* mem = (float*)malloc(sizeof(float)* n);
  for (int i = 0; i < n; i++)
      mem[i] = rand()%1000;
  float average = avg(mem, n);
  free(mem);
  return average;
}

int main(void) {
  struct sockaddr_in si_me, si_other;
  int s;
  socklen_t slen = sizeof(si_other);
  char buf[BUFLEN];
  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  C(s);
  int enable = 1;
  C(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)));

  memset((char *)&si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  C(bind(s, (struct sockaddr *)&si_me, sizeof(si_me)));
  while (1) {

    memset(buf, 0, sizeof(char) * BUFLEN);
    printf("Listening...\n");
    fflush(stdout);
    int recvlen =
        recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen);
    C(recvlen);
    printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr),
           ntohs(si_other.sin_port));
    printf("Received data:'%s'\n", buf);
    int seed = atoi(buf);
    float avgf = work(seed);
    // if (sscanf(buf, "%d\n", &s) != 1)
    //{
    printf("SEED = %d\nAVG  = %f\n", seed, avgf);
    sprintf(buf, "SEED = %d\nAVG  = %f\n", seed, avgf);
    C(sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&si_other, slen));

    /*  continue;
    }
    float avg = 1488.0f;
    printf( "AVERAGE : %f SEED : %d\n", avg, seed);

    char* replybuf = (char*)malloc(BUFLEN * sizeof(char));
    sprintf(replybuf,"All ok\n");
    C(sendto(s,"OK", 3, 0, (struct sockaddr*)&si_other, slen ));
    */
  }
  close(0);
  return 0;
}
