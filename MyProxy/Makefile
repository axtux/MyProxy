client:
	gcc client.c get_int.c -o client

server:
	gcc main.c network.c get_int.c -pthread -o server
clean:
	rm -f client server

