#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int serviceClient(int screenDescriptor);

void error(const char*msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		fprintf(stderr,"port number requires");
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;

	socklen_t clilen;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		error("Error openning socket");
	}

	bzero((char *)&serv_addr,sizeof(serv_addr));

	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0)
	{
		error("binding failed");
	}

	listen(sockfd,5);
	clilen=	sizeof(cli_addr);
	


	while(1){
		newsockfd = accept(sockfd,(struct sockaddr*) &cli_addr, &clilen);
		if(newsockfd<0)
		{
			error("Error on accepting");
		}
		printf("Connection Established with client\n");
		if(!fork()){
			fprintf(stderr, "serviceClient method will handle the Client\n");
			serviceClient(newsockfd);
		}
	}
	close(sockfd);
	return 0;
	
}

int serviceClient(int screenDescriptor){
	char msgBuffer[1000];
        dup2(screenDescriptor,1);
	while(1){
		fprintf(stderr,"Server Listening for client's command.........\n");
		if(!read(screenDescriptor, msgBuffer, 255)){
			close(screenDescriptor);
			fprintf(stderr,"Waiting for a new client\n");
			exit(0);
		}
		fprintf(stderr,"Client's Command is : %s\n",msgBuffer);
		int i=strncmp("quit",msgBuffer,3);
		if(i==0)
		{
			fprintf(stderr,"Client typed quit\n");
			fprintf(stderr,"Client requested for closing connection\n");
			close(screenDescriptor);
			fprintf(stderr,"Connection terminated by server\n");
			break;
		}
		system(msgBuffer);
		memset(&msgBuffer[0], 0, sizeof(msgBuffer));
		fprintf(stderr, "Command result showed successfully.\n");
	}
	return 0;
}
