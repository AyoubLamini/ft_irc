#include "../Includes/Bot.hpp"
#include "../Includes/WeatherData.hpp"
#include "../Includes/Server.hpp"


#include <algorithm> // for std::transform
#include <cctype>   // for std::tolower
#include <iostream>
#include <sstream>  // for std::stringstream
#include <unistd.h> // for close
#include <cstring> // for memset
#include <arpa/inet.h> // for inet_pton
#include <sys/socket.h> // for socket
#include <netinet/in.h> // for sockaddr_in
#include <fcntl.h> // for fcntl
#include <poll.h> // for poll
#include <vector> // for std::vector
#include <string> // for std::string
#include <map> // for std::map
#include <ctime> // for std::time
#include <cstdlib> // for std::rand
#include <netdb.h> // for gethostbyname
#include <sys/_endian.h>

Bot::Bot(int port, std::string hostname, std::string password) : serverPort(port), serverHostname(hostname), password(password)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

Bot::~Bot()
{
    close(sockfd);
}

void Bot::connectToServer()
{
    struct hostent		*server;
	struct sockaddr_in	serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        std::cerr << "\033[1;31m" << "socket() failed" << "\033[0m" << std::endl;
        exit(EXIT_FAILURE);
    }
	server = gethostbyname(serverHostname.c_str());
	if (!server)
		throw std::runtime_error("gethostbyname() failed");
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serverPort);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
        std::cerr << "\033[1;31m" << "connect() failed" << "\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    std::cout << "\033[1;32mConnected to IRC server at " << serverHostname << ":" << serverPort << "\033[0m" << std::endl;
}

void Bot::authenticate(std::string username, std::string nickname)
{
    std::string passwordMessage = "PASS " + password + "\r\n";
    std::string userMessage = "USER " + username + " 0 * :" + username + "\r\n";
    std::string nickMessage = "NICK " + nickname + "\r\n";
    send(sockfd, passwordMessage.c_str(), passwordMessage.length(), 0);
    usleep(90);
    send(sockfd, userMessage.c_str(), userMessage.length(), 0);
    usleep(90);
    send(sockfd, nickMessage.c_str(), nickMessage.length(), 0);
}

std::string Bot::getNickname(std::string message)
{
    std::string nickname;

    size_t pos = message.find("!");
    if (pos != std::string::npos)
    {
        nickname = message.substr(1, pos);
    }
    return nickname;
}

std::string Bot::getCityName(std::string message)
{
    std::string city;
    std::string delimiter = "PRIVMSG weather :";

    size_t pos = message.find(delimiter);
    if (pos != std::string::npos) {
        city = message.substr(pos + delimiter.length());
    }
    return city;
}

void Bot::respondToWeatherRequest(int sockfd, const std::string& cityName, const std::string& nickname)
{
    if (nickname == "weather" || nickname.empty() || cityName.empty())
        return ;
    CityWeather  cities[] = CITIES_WEATHER_MAP_INIT;
    std::string response;
    for (int i = 0; i < CITIES_NUMBER; ++i) {
        if (cities[i].city == cityName) {
            response = cities[i].weather;
            break;
        }
    }
    if (response.empty()) {
        response = "City not found!";
    }
    std::string message = ":" + nickname + "!" + nickname + "@localhost PRIVMSG " + nickname + " :" + response + "\r\n";
    send(sockfd, message.c_str(), message.length(), 0);
    std::cout << std::endl << "\033[1;35m" << response << "\033[0m" << std::endl << std::endl;
    std::cout << "\033[1;34m" << "-----------------------------------------" << "\033[0m" << std::endl;
    std::cout << "\033[1;33m" << "Check weather of your city 🌧️ 🌞 🌤️ 🌥️" << "\033[0m" << std::endl;
    std::cout << "\033[36m" << "Usage: PRIVMSG weather :<city_name>" << "\033[36m" << std::endl << std::endl;
}

void Bot::startBot()
{
    std::string userNickname;
    std::string message;
    std::string cityName;
    std::cout <<  "\033[1;34m" << "-----------------------------------------" << "\033[0m" << std::endl;
    std::cout << "Bot started. Waiting for messages...\033[0m" << std::endl;
    std::cout << "\033[36m" << "Usage: PRIVMSG weather :<city_name>"<<  "\033[36m" << std::endl;      
    while (true) {
        char buffer[1024];
        ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            perror("recv");
            break;
        } else if (bytesReceived == 0) {
            std::cout << "\033[1;31mServer closed the connection.\033[0m" << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0';
        message = std::string(buffer);
        if (hasCR(message))
            message = message.substr(0, message.find("\r"));
        if (has_newline(message))
            message = message.substr(0, message.find("\n"));

        userNickname = getNickname(message);
        cityName = getCityName(message);
        // std::cout << "nickname: " << userNickname << std::endl;
        // std::cout << "city length: " << cityName.length() << std::endl;
        // std::cout << "cityName: |" << cityName << "|" << std::endl;
        respondToWeatherRequest(sockfd, cityName, userNickname);
    }
}