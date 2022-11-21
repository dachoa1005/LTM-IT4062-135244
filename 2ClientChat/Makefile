all: server client1 client2
server.o: server.c
	gcc -c server.c
server: server.o
	gcc -o server server.o
client1.o: client1.c
	gcc -c client1.c
client1: client1.o
	gcc -o client1 client1.o
client2.o: client2.c
	gcc -c client2.c
client2: client2.o
	gcc -o client2 client2.o
clean:
	rm -f *.o server client1 client2