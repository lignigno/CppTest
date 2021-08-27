/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SimpleClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 18:39:14 by lignigno          #+#    #+#             */
/*   Updated: 2021/08/21 21:12:39 by lignigno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "header.hpp"

static bool	get_command(std::string &command)
{
	std::getline(std::cin, command, '\n');
	if (command.length() == 0)
		command = "{1001}";
	if (std::cin.eof())
		return false;
	for (size_t i = 0; command[i]; i++)
		if ('a' <= command[i] && command[i] <= 'z')
			command[i] = (char)(command[i] - ' ');
	return true;
}

int main(int count, char **values)
{
	std::cout << "\nCLIENT START\n\n";
	struct sockaddr_in sa;
	int res;
	int SocketFD;

	SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SocketFD == -1)
	{
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}

	memset(&sa, 0, sizeof sa);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(1100);
	res = inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);

		if (connect(SocketFD, (struct sockaddr *)&sa, sizeof(sa)) == -1)
		{
			perror("connect failed");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}

	while (1)
	{
		u_char	*buff = new u_char[BUFF_SIZE];
		int		readed;


		std::cout << COL_Y << "enter message or exit" << US << "\n";
		std::string command;

		if (!get_command(command))
		{
			close(SocketFD);
			std::cout << "\nCLIENT GOT EOF\n\n";
			exit (0);
		}
		if (command == "EXIT")
			break ;
		write(SocketFD, command.c_str(), command.length());
		readed = read(SocketFD, buff, BUFF_SIZE);
		buff[readed] = '\0';
		printf("server said {%s}\n", buff);
	}
	std::cout << "\nCLIENT FINISH\n\n";
	close(SocketFD);
	return EXIT_SUCCESS;
}
