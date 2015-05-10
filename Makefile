client: rm_client
	gcc -g client.c network.c functions.c -o client
server: rm_server
	gcc -g main.c network.c functions.c -pthread -o server
	mkdir cache_files
test: rm_test
	gcc -g test.c functions.c -o test
clean: rm_client rm_server rm_test
	rm -rf cache_files
rm_client:
	rm -f client
rm_server:
	rm -f server
rm_test:
	rm -f test

