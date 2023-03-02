#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define RESET(buff)	\
	for(int i=0; i<100; i++)	\
		buff[i] = '\0';
int main(int argc, char *argv[]){
	if(argc < 3){
		printf("[+] Usage: ./server ip port\n");
		exit(1);
	}
	char *ip = argv[1];
	char *p = argv[2];
	int port = atoi(p);
	//char *name = argv[2];

	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char msg[100];
	int n;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock < 0){
		perror("[-]Socket error");
		exit(1);
	}
	//printf("[+]TCP server socket created!\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(n < 0){
		perror("[-] Bind error");
		exit(1);
	}
	//printf("[+] Bind to the port: %d\n", port);
	printf("\n=================================================\n");
	printf("******************SHELLCHAT v1.0*****************\n");
	printf("=================================================\n");
	listen(server_sock, 5);
	//printf("Listening...\n");
	printf("[+] Waiting for client to join...\n");
	

	addr_size = sizeof(client_addr);
	client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
	printf("[+] Client Joined!\n\n");
	while(1){
		//bzero(msg, 1024);
		RESET(msg)
		recv(client_sock, msg, sizeof(msg), 0);

		if(strncmp("Client quit!", msg, 12)==0){
			printf("%s\n", msg);
			//send(client_sock, msg, strlen(msg), 0);
			break;
		}
		printf("Client: %s\n", msg);
		//printf("%s\n", msg);
		n = 0;
		RESET(msg)
		printf("Server: ");
		while((msg[n++] = getchar()) != '\n');

		if(strncmp("q", msg, 1)==0 || strncmp("Q", msg, 1)==0){
			char exit_msg[20] = "Server quit!\n";
			send(client_sock, exit_msg, strlen(exit_msg), 0);
			break;
		}

		send(client_sock, msg, strlen(msg), 0);
		
	}
	close(client_sock);
	printf("[+] Disconnected!\n");
	return 0;
}

