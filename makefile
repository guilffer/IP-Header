all: client server

client: libraw.a client.c
	gcc -Wall client.c -o client -L. -lraw

server: libraw.a server.c
	gcc -Wall server.c -o server -L. -lraw

libraw.a: raw_socket.c
	gcc -Wall -c raw_socket.c
	ar rvs libraw.a raw_socket.o
	ranlib libraw.a

clean:
	rm -rf *.o *.a client server
