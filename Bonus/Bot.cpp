/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:33:44 by ybouyzem          #+#    #+#             */
/*   Updated: 2025/05/30 15:34:59 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Bot.hpp"
#include "../Includes/WeatherData.hpp"
#include "../Includes/Server.hpp"


#include <algorithm> 
#include <cctype>   
#include <iostream>
#include <sstream>  
#include <unistd.h> 
#include <cstring> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#include <poll.h> 
#include <vector> 
#include <string> 
#include <map> 
#include <ctime> 
#include <cstdlib> 
#include <netdb.h> 

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
    struct hostent *server;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        std::cerr << "\033[1;31msocket() failed\033[0m" << std::endl;
        exit(EXIT_FAILURE);
    }

    server = gethostbyname(serverHostname.c_str());
    if (!server)
    {
        std::cerr << "\033[1;31mError: Invalid hostname '" << serverHostname << "'\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(serverPort);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "\033[1;31mConnection to the IRC server failed\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    std::cout << "\033[1;32mConnected to IRC server at " << serverHostname << ":" << serverPort << "\033[0m" << std::endl;
}


void Bot::authenticate(std::string username, std::string nickname)
{
    if (username.empty() || nickname.empty())
    {
        std::cerr << "\033[1;31mUsername and nickname cannot be empty\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    
    std::string passwordMessage = "PASS " + password + "\r\n";
    std::string userMessage = "USER " + username + " 0 * :" + username + "\r\n";
    std::string nickMessage = "NICK " + nickname + "\r\n";
    if (send(sockfd, passwordMessage.c_str(), passwordMessage.length(), 0) < 0)
    {
        std::cerr << "\033[1;31m" << "send() failed" << "\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    usleep(90);
    if (send(sockfd, userMessage.c_str(), userMessage.length(), 0) < 0)
    {
        std::cerr << "\033[1;31m" << "send() failed" << "\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    usleep(90);
    if (send(sockfd, nickMessage.c_str(), nickMessage.length(), 0) < 0)
    {
        std::cerr << "\033[1;31m" << "send() failed" << "\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    std::string response;
    char buffer[1024];
    ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        perror("recv");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[bytesReceived] = '\0';
    response = std::string(buffer);
    if (response.find("Nickname is already in use") != std::string::npos)
    {
        std::cerr << "\033[1;31m" << "Registration failed" << "\033[0m" << std::endl;
        close(sockfd);
        exit(EXIT_FAILURE);
    }
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
        respondToWeatherRequest(sockfd, cityName, userNickname);
    }
}