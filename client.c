#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define PORT 9930
#define SRV_IP "127.0.0.1"

void fatal(char *s) {
  perror(s);
  exit(1);
}

int main(void)
{
  struct sockaddr_in si_other;
  int s, e, slen=sizeof(si_other);
  char buf[BUFLEN];

  s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (s < 0) fatal("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr)==0)
    fatal("inet_aton");

  printf("Sending packet");
  sprintf(buf, "This is packet");
  e = sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen);
  if (e < 0) fatal("sendto");

  close(s);
  return 0;
}

