#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int ac, char **av)
{
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int sockfd = 0;
	int newsockfd = 0;
	int portno = 0;
	int clilen = 0;
	int n = 0;
	if (ac < 2)
	{
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	bzero((char*) &serv_addr, sizeof(serv_addr));
	portno = atoi(av[1]);

	serv_addr.sin_family = AF_INET;				//
	serv_addr.sin_addr.s_addr = INADDR_ANY;		//
	serv_addr.sin_port = htons(portno);			//
/*
**	 bind(int socket, const struct sockaddr *address, socklen_t address_len);
**	--------------------------------------------------------------------------
**	bind() assigns a name to an unnamed socket.  When a socket is created with socket(2) it exists
**	     in a name space (address family) but has no name assigned.  bind() requests that address be
**	     assigned to the socket
*/
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd, 5);

	clilen = sizeof(cli_addr);

	/** 	accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
	**		------------------------------------------------------
	**
	**/
	newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);

	if (newsockfd < 0)
		error("ERROR reading from socket");

	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket");
	printf("msg:%s\n", buffer);
	n = write(newsockfd, "I got your message", 18);
	if (n < 0)
		error("ERROR writing from socket");
	return (0);
}
