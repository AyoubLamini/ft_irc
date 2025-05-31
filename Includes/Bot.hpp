/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:30:11 by ybouyzem          #+#    #+#             */
/*   Updated: 2025/05/31 16:09:23 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../Includes/WeatherData.hpp" 
#include <vector>
#include <map> 
#include <algorithm> 
#include <cctype>   
#include <sstream>  
#include <unistd.h> 
#include <cstring> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#include <poll.h> 
#include <string> 
#include <ctime> 
#include <cstdlib> 
#include <netdb.h> 

struct user
{
	std::string nickname;
	bool waitingForCity;
};

class Bot {
	private:

		std::map<std::string, std::string> weatherData; 
		int serverPort;
		int sockfd;
		std::string serverHostname;
		std::string password;
		std::vector<user> users;
	public:
		Bot();
		Bot(int port, std::string hostname, std::string password);
		~Bot();
		void connectToServer();
		void authenticate(std::string username, std::string nickname);
		void startBot();
		std::string getCityName(std::string message);
		std::string getNickname(std::string message);
		void respondToWeatherRequest(int sockfd, const std::string& cityName, const std::string& nickname);

};

#endif
