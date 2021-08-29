/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_3.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 16:26:16 by lignigno          #+#    #+#             */
/*   Updated: 2021/08/30 03:55:14 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../header.hpp"

int	getServerSock(void)
{
	struct sockaddr_in	sa;
	int					SocketFD;

	SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	return SocketFD;
}

void	InitialSetup(struct pollfd fds[], int count)
{
	for (size_t i = 0; i < count; i++)
		fds[i].fd = -1;
}

int main(int argc, char const *argv[])
{
	int				CountConnects = 0;
	struct pollfd	fds[10];

	InitialSetup(fds, 10);
	fds[0].fd = getServerSock();
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	CountConnects++;

	int flag = 0;
	while (true)
	{
		int CountFD;

		if (flag > 0)
		{
			std::cout << "exit\n";
			exit(EXIT_SUCCESS);
		}
		std::cout << COL_Y << "before" << US << "\n";
		if (CountFD = poll(fds, CountConnects, -1) < 0)
		{
			perror("POLL CRASH");
			exit(EXIT_FAILURE);
		}
		std::cout << COL_G << "after " << CountFD << US << "\n";

		// std::cout << "whait\n";
		// sleep(1);
		// std::cout << "go\n";

		for (size_t i = 0; i < CountConnects; i++)
		{
			if (fds[i].fd > 0 && (fds[i].revents & POLLIN) == POLLIN)
			{
			++flag;
				if (!i)
				{
					/*new connect*/
					flag = 0;

					std::cout << CountConnects << " " << flag << '\n';
					fds[CountConnects].fd = accept(fds[i].fd, NULL, NULL);
					std::cout << COL_R << "NEW CONNECT" << US << '\n';
					fds[CountConnects].events = POLLIN;
					fds[CountConnects].revents = 0;
					++CountConnects;
				}
				else
				{
					/*caught a squirrel*/
					char	buff[BUFF_SIZE];
					flag = 0;
					// if ((fds[i].revents & POLLHUP) == POLLHUP)
					// {
					// 	fds[i].fd = -1;

					// 	--CountConnects;
					// 	continue ;
					// }
					int readed;

					readed = read(fds[i].fd, buff, BUFF_SIZE);
					fds[i].revents = 0;
					std::cout << COL_B << "MESSAGE " << i << US << '\n';
					if (!readed)
					{
						write(fds[i].fd, "end", 3);
						write(1, "end\n", 4);
						fds[i].fd = -1;
						--CountConnects;
						continue ;
					}
					buff[readed] = 0;
					write(fds[i].fd, "i got message", 13);

					printf("{%s}\n", buff);
					fds[i].revents = 0;
				}
			}
		}
	}

	return 0;
}
