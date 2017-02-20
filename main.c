/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hvillasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/16 12:31:08 by hvillasa          #+#    #+#             */
/*   Updated: 2017/02/17 23:37:24 by hvillasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "libft/libft.h"

static void	*error(char *msg)
{
	ft_putstr("ERROR: ");
	ft_putendl(msg);
	return (0);
}

static void	bind_sock(int sockfd, int portno, struct sockaddr_in serv_addr)
{
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		error("on binding socket");
}

static int	accept_sock(int sockfd, struct sockaddr_in *cli_addr)
{
	int client_len;
	int newsock_fd;

	client_len = sizeof(cli_addr);
	newsock_fd = accept(sockfd, (struct sockaddr *)&cli_addr,
			(socklen_t *)&client_len);
	if (newsock_fd < 0)
		error("ERROR on accept");
	return (newsock_fd);
}

static void	read_ping(int sockfd, struct sockaddr_in *cli_addr)
{
	int		n;
	char	buffer[256];
	int 	newsock_fd;	
	while (1)
	{	
		newsock_fd = accept_sock(sockfd, cli_addr);
		ft_bzero(buffer, 256);
		n = read(newsock_fd, buffer, 255);
		if (n > 0)
		{
			write(newsock_fd, buffer, n);
			write(1, buffer, n);
		}
		else
			error("reading from socket");
		close(newsock_fd);
	}
}

int			main(int ac, char **av)
{
	int			yes = 1;
	int			sockfd;
	int			portno;
	struct sockaddr_in	serv_addr;
	struct sockaddr_in	cli_addr;

	if (ac != 2)
	{
		error("no port provided");
		return (0);
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("opening socket");
	if (setsockopt(sockfd, SOCK_STREAM, SO_REUSEADDR, &yes, sizeof(int) == 1) == 1)
		error("setting socket option so_reuseaddr");
	ft_bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = ft_atoi(av[1]);
	bind_sock(sockfd, portno, serv_addr);
	listen(sockfd, 5);
	read_ping(sockfd,  &cli_addr);
	close(sockfd);
	return (0);
}
