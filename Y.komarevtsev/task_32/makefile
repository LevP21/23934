all: client server clientC

client: client.c
	gcc -o client client.c -lsocket -lnsl

server: server.c
	gcc -o server server.c -lsocket -lnsl

clientC: Writer.c
	gcc -o writer Writer.c -lsocket -lnsl

clear:
	rm client 
	rm server
	rm /tmp/socket32
