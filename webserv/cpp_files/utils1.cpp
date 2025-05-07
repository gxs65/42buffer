/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:09:53 by ilevy             #+#    #+#             */
/*   Updated: 2025/05/06 11:37:30 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp_files/webserv.hpp"

// Displays a user-defined error message <msg>,
// and the errno message if <displayErrno> is true
int	logError(std::string msg, bool displayErrno)
{
	std::cout << msg << "\n";
	if (displayErrno)
		perror("Error description");
	return (1);
}

// Displays the current hour as HH:MM:SS
void	logTime(int linebreak)
{
	time_t				timestamp = time(NULL);
	struct tm*			datetime;

	datetime = localtime(&timestamp);
	if (datetime->tm_hour < 10)
		std::cout << "0";
	std::cout << datetime->tm_hour << ":";
	if (datetime->tm_min < 10)
		std::cout << "0";
	std::cout << datetime->tm_min << ":";
	if (datetime->tm_sec < 10)
		std::cout << "0";
	std::cout << datetime->tm_sec;
	if (linebreak)
		std::cout << "\n";
	else
		std::cout << " ";
}
