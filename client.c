#include <stdio.h>
#include <stdlib.h>
#include "raw_socket.h"

int main(int argc, char ** argv) {
  char *recv_msg = NULL;
  char *source = NULL;

  if (argc != 3) {
    printf("%s server-ip message\n", argv[0]);
    exit(1);
  }

  do_client(argv[1], argv[2]);
  do_server(&source, &recv_msg);
  printf("\nReceived Message = '%s'\n",recv_msg);
  return 0;
}
