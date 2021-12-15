CC = gcc

server: server.c 
	$(CC) server.c handle.c serverfunction.h serverfunction.c -o server -Wall -lpthread

clean:
	rm -rf *.o