/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyFirstSteps.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/20 18:02:27 by lignigno          #+#    #+#             */
/*   Updated: 2021/08/22 00:25:43 by lignigno         ###   ########.fr       */
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
#include "header.hpp"

int	InitialSetup(void)
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

//                                                                            |
// ---------------------------------------------------------------------------|
//                                                                            |

int main(void)
{
	int		SocketFD, ConnectFD;
	int	CountFD;
	fd_set	RSockets, CSockets;
	u_char	*buff = new u_char[BUFF_SIZE];

	std::cout << "\nSTART\n\n";

	SocketFD = InitialSetup();
	FD_ZERO(&CSockets);
	FD_SET(SocketFD, &CSockets);
		int flag = 0;
	while (true)
	{
		RSockets = CSockets;
		std::cout << COL_Y << "before" << US << "\n";
		CountFD = select(FD_SETSIZE, &RSockets, NULL, NULL, NULL);
		std::cout << COL_G << "after" << US << "\n";
		if (CountFD < 0)
		{
			perror("select failed");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}
		std::cout << CountFD << "\n";
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &RSockets))
			{
				if (i == SocketFD)
				{
					std::cout << COL_B << "this" << US << "\n";
					ConnectFD = accept(SocketFD, NULL, NULL);
					if (ConnectFD == -1)
					{
						perror("accept failed");
						close(SocketFD);
						exit(EXIT_FAILURE);
					}
					FD_SET(ConnectFD, &CSockets);
				}
				else
				{
					std::cout << COL_R << "this" << US << "\n";
					int	readed = read(i, buff, BUFF_SIZE);
					if (readed < 0)
						FD_CLR(i, &CSockets);
					else
					{
						std::cout << "number readed chars " << readed << "\n";
						if (flag > 0) exit(0);
						flag++;
						if (!readed)
						{
							FD_CLR(i, &CSockets);
							break ;
						}
						flag = 0;
						buff[readed] = '\0';
						write(ConnectFD, "I got message", 13);
						printf("%s\n", buff);
					}
				}
			}
		}
	}
	return 0;
}
