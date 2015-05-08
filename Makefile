client: rm_client
	gcc -g client.c network.c functions.c -o client

server: rm_server
	gcc -g main.c network.c functions.c -pthread -o server
clean:
	rm -f client server
rm_client:
	rm -f client
rm_server:
	rm -f server

