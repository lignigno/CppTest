/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lignigno <lignign@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/15 18:39:14 by lignigno          #+#    #+#             */
/*   Updated: 2021/08/30 03:54:20 by lignigno         ###   ########.fr       */
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
#include "../header.hpp"

#define _PRINT_HEADER

// _________________________________________________________________SUBFUNCTIONS

static bool	get_msg(std::string &msg)
{
	std::getline(std::cin, msg, '\n');
	if (msg.length() == 0)
		msg = "{1001}";
	if (std::cin.eof())
		return false;
	for (size_t i = 0; msg[i]; i++)
		if ('a' <= msg[i] && msg[i] <= 'z')
			msg[i] = (char)(msg[i] - ' ');
	return true;
}

//                                                                             |
// ----------------------------------------------------------------------------|
//                                                                             |

#ifdef PRINT_HEADER

#define ONE "\e[48;2;55;255;0m_\e[m"
#define ZERO "\e[48;2;179;0;0m_\e[m"

static void	PrintHeader(const char *packege, int n)
{
	// for (size_t i = 0; i < 32; i++)
	// 	std::cout << "| ";
	// std::cout << '\n';
	
	std::cout << "{\n";
	for (size_t CharN = 0; CharN < n; CharN++)
	{
		if (packege[CharN] < ' ')
			std::cout << COL_B << "{" << (int)packege[CharN] << "}" << US;
		else
			std::cout << packege[CharN];
	}
	std::cout << "\n}\n";

	for (size_t CharN = 0; CharN < n; CharN++)
	{
		std::cout << (!(CharN % 4) ? "\n| " : "");
		for (size_t bit = 128; bit > 0; bit /= 2)
			std::cout << (((packege[CharN] & bit) / bit) ? ONE : ZERO) << ' ';
		std::cout << "| ";
	}
	std::cout << '\n';
}

#endif

//                                                                             |
// ----------------------------------------------------------------------------|
//                                                                             |

static uint16_t	get_port(void)
{
	uint16_t	port;

	std::cout << "enter port : ";
	std::cin >> port;
	return port;
}

//                                                                             |
// ----------------------------------------------------------------------------|
//                                                                             |

static std::string	get_ip(void)
{
	std::string	ip;
	std::string	tmp;

	std::cout << "enter ip   : ";
	std::cin >> ip;

	std::getline(std::cin, tmp, '\n');
	return ip;
}

// ____________________________________________________________________MAIN FUNC

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
	sa.sin_port = htons(get_port());
	res = inet_pton(AF_INET, get_ip().c_str(), &sa.sin_addr);

		if (connect(SocketFD, (struct sockaddr *)&sa, sizeof(sa)) == -1)
		{
			perror("connect failed");
			close(SocketFD);
			exit(EXIT_FAILURE);
		}

	while (1)
	{
		char	*buff = new char[BUFF_SIZE];
		int		readed;


		std::cout << COL_Y << "enter message or exit" << US << "\n";
		std::string msg;

		if (!get_msg(msg))
		{
			close(SocketFD);
			std::cout << "\nCLIENT GOT EOF\n\n";
			exit (0);
		}
		if (msg == "EXIT")
		{
			break ;
		}
		write(SocketFD, msg.c_str(), msg.length());
		readed = read(SocketFD, buff, BUFF_SIZE);
		buff[readed] = '\0';
		std::cout << "readed : " << readed << " bite\n";
		printf("server said {%s}\n", buff);

#ifdef PRINT_HEADER
		PrintHeader(buff, readed);
#endif

	}
	std::cout << "\nCLIENT FINISH\n\n";
	close(SocketFD);
	return EXIT_SUCCESS;
}
