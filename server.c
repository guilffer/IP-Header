#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void fatal(char *s) {
  perror(s);
  exit(1);
}

void print_ip_header(unsigned char* Buffer, int Size) {
  unsigned short iphdrlen;
  struct iphdr *iph = (struct iphdr *)Buffer;
  struct sockaddr_in source, dest;

  iphdrlen = iph->ihl*4;

  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = iph->saddr;

  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = iph->daddr;

  printf("\n");
  printf("IP Header\n");
  printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
  printf("   |-IP Header Length  : %d Bytes\n",((unsigned int)(iph->ihl))*4);
  printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
  printf("   |-IP Total Length   : %d Bytes\n",ntohs(iph->tot_len));
  printf("   |-Identification    : %d\n",ntohs(iph->id));
  printf("   |-Fragment Offset   : %d\n",ntohs(iph->frag_off));
  printf("   |-TTL               : %d\n",(unsigned int)iph->ttl);
  printf("   |-Protocol          : %d\n",(unsigned int)iph->protocol);
  printf("   |-Checksum          : %d\n",ntohs(iph->check));
  printf("   |-Source IP         : %s\n",inet_ntoa(source.sin_addr));
  printf("   |-Destination IP    : %s\n",inet_ntoa(dest.sin_addr));
}


int main(void) {
  struct sockaddr_in si_me, si_other;
  int s, i, b, r, slen=sizeof(si_other);
  char buf[BUFLEN];

  s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (s<0) fatal("socket");

  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  b = bind(s, (struct sockaddr *) &si_me, sizeof(si_me));
  if (b < 0) fatal("bind");

  r = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
  if (r < 0) fatal("recvfrom()");

  print_ip_header(buf,r);

  close(s);
  return 0;
}

