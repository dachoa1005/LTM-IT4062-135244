all: server client
server.o: server.c
	gcc -c server.c
server: server.o
	gcc -o server server.o
client.o: client.c
	gcc -c client.c
client: client.o
	gcc -o client client.o
clean:
	rm -f *.o server client