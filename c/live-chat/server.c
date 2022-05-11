#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[]){
	if(argc < 2){
		fprintf(stderr, "Port no not provided, Program terminated\n");
		exit(1);
	}
	int sockfd, newSockfd, portNo, n;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error opening Socket.");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portNo = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portNo);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		error("Binding failed.");
	}

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	newSockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newSockfd < 0){
		error("Error on Accept.");
	}

	while(1){
		bzero(buffer, 255);
		n = read(newSockfd, buffer, 255);
		if(n < 0){
			error("error on reading from server.");
		}
		printf("client: %s\n", buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(newSockfd, buffer, strlen(buffer));
		if(n < 0){
			error("error on writing from server");
		}
		if(!(strncmp("Bye", buffer, 3))){
			break;
		}
	}
	close(newSockfd);
	close(sockfd);

	return 0;
}
