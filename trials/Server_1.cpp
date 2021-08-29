/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_1.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/12 20:50:59 by lignigno          #+#    #+#             */
/*   Updated: 2021/08/28 15:52:26 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../header.hpp"

int main(void)
{
	std::cout << "\nSERVER START\n\n";
	struct sockaddr_in	sa;
	u_char				*buff = new u_char[BUFF_SIZE];

	int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SocketFD == -1)
	{
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(SocketFD,(struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		perror("bind failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	if (listen(SocketFD, 100) == -1)
	{
		perror("listen failed");
		close(SocketFD);
		exit(EXIT_FAILURE);
	}

	for (;;)
	{
		std::cout << COL_Y << "server waiting connect" << US << "\n";
		int	readed;
		int ConnectFD = accept(SocketFD, NULL, NULL);

		if (ConnectFD == -1)
		{
			perror("accept failed");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}
		
		std::cout << COL_G << "server get connect" << US << "\n";

		readed = read(ConnectFD, buff, BUFF_SIZE);
		if (readed < 0)
		{
			perror("receiving failed");
			close(ConnectFD);
			close(SocketFD);
			exit(EXIT_FAILURE);
		}

		if (!readed)
			break ;

		std::cout << "number readed chars " << readed << "\n";
		buff[readed] = '\0';
		write(ConnectFD, "I got message", 13);
		printf("%s\n", buff);

		if (shutdown(ConnectFD, SHUT_RDWR) == -1)
		{
			perror("shutdown failed");
			close(ConnectFD);
			close(SocketFD);
			exit(EXIT_FAILURE);
		}
		// close(ConnectFD);
	}

	close(SocketFD);
	return EXIT_SUCCESS;
}
