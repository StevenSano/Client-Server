#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int ac, char **av)
{
	char buffer[256];
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int sockfd = 0;
	int portno = 0;
	int n = 0;

	if (ac < 3)
	{
		fprintf(stderr, "usage %s hotstname port\n", av[0]);
		exit(1);
	}
	portno = atoi(av[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(av[1]);
	if (server == NULL)
	{
		fprintf(stderr, "%s\n", "ERROR, no such host\n");
		exit(1);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
/*
**	The bcopy() function copies len bytes from string src to string dst.  The two
**	     strings may overlap.  If len is zero, no bytes are copied.
*/
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);

	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	printf("Please eneter message: ");
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
	{
		error("ERROR writing to socket");
	}
	bzero(buffer, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
		error ("ERROR reading from socket");
	printf("%s\n", buffer);
	return (0);
}
