#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 1024
#define APPPORT 8550

void fatal(char *s) {
  perror(s);
  exit(1);
}

void print_ip_header(char *buffer) {
  struct iphdr *iph = (struct iphdr *)buffer;
  struct sockaddr_in source, dest;

  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = iph->saddr;

  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = iph->daddr;

  printf("\n");
  printf("IP Header\n");
  printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
  printf("   |-IP Header Length  : %d\n",((unsigned int)(iph->ihl))*4);
  printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
  printf("   |-IP Total Length   : %d\n",ntohs(iph->tot_len));
  printf("   |-Identification    : %d\n",ntohs(iph->id));
  printf("   |-Fragment Offset   : %d\n",ntohs(iph->frag_off));
  printf("   |-TTL               : %d\n",(unsigned int)iph->ttl);
  printf("   |-Protocol          : %d\n",(unsigned int)iph->protocol);
  printf("   |-Checksum          : %d\n",ntohs(iph->check));
  printf("   |-Source IP         : %s\n",inet_ntoa(source.sin_addr));
  printf("   |-Destination IP    : %s\n",inet_ntoa(dest.sin_addr));
}

char * get_source_addr(char *buffer) {
  struct iphdr *iph = (struct iphdr *)buffer;
  struct sockaddr_in source;

  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = iph->saddr;
  return inet_ntoa(source.sin_addr);
}


char * get_payload(char *buffer) {
  struct iphdr *iph = (struct iphdr *)buffer;
  struct udphdr *udph = (struct udphdr*)(buffer + (iph->ihl*4));
  return (char *)(buffer + (iph->ihl*4) + sizeof(udph) - 4);
}

void do_server(char **source_addr, char **recv_payload) {
  struct sockaddr_in si_me, si_other;
  int s, b, size;
  socklen_t slen=sizeof(si_other);
  char buffer[BUFLEN];

  s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (s<0) fatal("socket");

  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(APPPORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  b = bind(s, (struct sockaddr *) &si_me, sizeof(si_me));
  if (b < 0) fatal("bind");

  size = recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
  if (size < 0) fatal("recvfrom()");

  print_ip_header(buffer);
  *source_addr = get_source_addr(buffer);
  *recv_payload = get_payload(buffer);

  close(s);
}

void do_client(char *dest, char *message) {
  struct sockaddr_in si_other;
  int s, e, slen=sizeof(si_other);
  char buf[BUFLEN];

  sleep(2);

  s = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (s < 0) fatal("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(APPPORT);
  if (inet_aton(dest, &si_other.sin_addr)==0) fatal("inet_aton");

  printf("Sending packet\n");
  sprintf(buf, "%s", message);
  e = sendto(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, slen);
  if (e < 0) fatal("sendto");

  close(s);
}
