#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#define RESET(buff)	\
	for(int i=0; i<100; i++) \
		buff[i] = '\0';
int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Please specify the port!\n");
		exit(1);
	}
	char *ip = "127.0.0.1";
	char *p = argv[1];
	int port = atoi(p);

	int sock;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char msg[100];
	int n;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("[-] Socket error");
		exit(1);
	}

	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(ip);

	connect(sock, (struct sockaddr*)&addr, sizeof(addr));
	printf("\n=================================================\n");
	printf("*****************SHELLCHAT v1.0******************\n");
	printf("=================================================\n");

	printf("[+] Connected to the server.\n\n");
	
	while(1){
		//bzero(msg, 1024);
		RESET(msg)
		int n = 0;
		printf("Client: ");
		while ((msg[n++] = getchar()) != '\n');
		
		if(strncmp("q", msg, 1)==0 || strncmp("Q", msg, 1)==0){
			char exit_msg[20] = "Client quit!\n";
			send(sock, exit_msg, strlen(exit_msg), 0);
			break;
		}
		send(sock, msg, strlen(msg), 0);
		
		RESET(msg)
		recv(sock, msg, sizeof(msg), 0);

		if(strncmp("Server quit!", msg, 12)==0){
			printf("%s\n", msg);
			break;
		}
		printf("Server: %s\n", msg);
	}
	
	close(sock);
	printf("[+] Disconnected!\n");
	return 0;
}
