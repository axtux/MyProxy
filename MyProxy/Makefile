client: rm_client
	gcc client.c get_int.c -o client

server: rm_server
	gcc main.c network.c get_int.c -pthread -o server
clean:
	rm -f client server
rm_client:
	rm -f client
rm_server:
	rm -f server

