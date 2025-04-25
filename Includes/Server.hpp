#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <poll.h>
#include <arpa/inet.h> // for htons
#include <fcntl.h>
#include <sstream> // for std::stringstream

#include "Channel.hpp"
#include "Client.hpp"

class Client;

class Channel;

class Server
{
    private:
        int port;
        int server_fd;
        std::string password;
        std::vector <Client *> _Clients;
        std::vector <Channel *> _Channels;    
        std::vector <pollfd> poll_fds;

    public:
    Server();
    ~Server();

    // Getters / Setters
    int getPort() const;
    std::string getPassword() const;
    void setPassword(std::string password);
    void setPort(int port);
    int getServerFd() const;
    void setServerFd(int server_fd);
    Client *getClientByFd(int client_fd);



    // phases--------------------------------------------- 

   // Setup
    void initializeServer();
    void run();
    void acceptClient();
    void readClient(int client_fd);
    std::string recvMessage(int client_fd);
    // Authentication / Registration
    void authenticateClient(Client *client, const std::vector<std::string>& tokens);
    void registerClient(Client *client, const std::vector<std::string>& tokens);
    void checkRegistration(Client *client);


    // Send data
    void prepareSendBuffer(int client_fd, const std::string& message);
    void writeClient(int client_fd);

    // Proccess Commands / Messages

    void processCommands(Client *client, const std::vector <std::string>& tokens);
    void joinMessage(Client *client, const std::vector <std::string>& tokens);
    

  
    // Channel managment

    bool channelExist(std::string channel);
    Channel *getChannel(std::string channel);
    void createChannel(Client *client, std::string name, std::string key);
    void joinChannel(Client *client, std::string name, std::string key);

//   utils
    std::vector<std::string> splitedInput(const std::string& input, char delimiter);
    bool inCommandslist(std::string command);
    bool isValidNickname(const std::string& nickname) ;
    void printMessage(const std::vector<std::string>& tokens);
    bool nickExists(std::string nickname);
    std::vector<std::string> splitedJoin(const std::string& input);

    // Free and cleanup
    void ClearDisconnectedClients();
    void deleteClientData(Client *client);
    bool startsWith(const std::string& str, const std::string& set) {return !str.empty() && set.find(str[0]) != std::string::npos;}

};



#endif