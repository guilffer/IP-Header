#include <stdio.h>
#include <stdlib.h>
#include "raw_socket.h"

int main(void) {
  char *recv_msg = NULL;
  char *source = NULL;
  do_server(&source, &recv_msg);
  printf("\nreceived message = %s\n",recv_msg);
  do_client(source, recv_msg);
  return 0;
}
