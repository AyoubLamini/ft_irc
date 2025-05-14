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
struct user
{
	std::string nickname;
	bool waitingForCity; // Whether the player is waiting for a city name
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
