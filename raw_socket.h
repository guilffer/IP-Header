#define BUFLEN 1024
#define APPPORT 29811

void fatal(char *s);
void print_ip_header(char *buffer);
int  verify_port(char * buffer);
char * get_source_addr(char *buffer);
char * get_payload(char *buffer);
void do_server(char **source_addr, char **recv_payload);
void do_client(char *dest, char *message);
