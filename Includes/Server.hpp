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
    Client *getClientByNickname(const std::string& nickname);



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
    void respond(int client_fd, const std::string& message);
    void writeClient(int client_fd);

    // Proccess Commands / Messages

    void processCommands(Client *client, const std::vector <std::string>& tokens, std::string msg);
    void joinMessage(Client *client, const std::vector <std::string>& tokens);
    void privateMessage(Client *client, std::string msg);
    void sendMessageToClient(Client *sender, std::string targetName, std::string msg);
    void inviteToChannel(Client *client, const std::vector <std::string>& tokens);
    void channelMode(Client *client, const std::vector <std::string> &tokens);
    int validateModes(Client *client, Channel *channel, std::string modes, const std::vector <std::string> &tokens);
    int executeMode(Client *client, Channel *channel, char mode, char sign, const std::vector <std::string> &tokens, size_t *counter);
    

  
    // Channel managment

    bool channelExist(std::string channel);
    Channel *getChannel(std::string channel);
    void createChannel(Client *client, std::string name, std::string key);
    void deleteEpmtyChannels();
    void joinChannel(Client *client, std::string name, std::string key);
    void sendMessageToChannel(Client *sender, std::string channelName, std::string message, const std::string& messageType);








//   utils
    std::vector<std::string> splitedInput(const std::string& input, char delimiter);
    std::vector<std::string> topicSplit(const std::string& input);
    bool inCommandslist(std::string command);
    bool isValidNickname(const std::string& nickname) ;
    bool isValidChannelKey(const std::string& key);
    void printMessage(const std::vector<std::string>& tokens);
    bool nickExists(std::string nickname);
    std::string formatIrcMessage(const std::string& prefixNick, const std::string& prefixUser, const std::string& command, const std::string& target, const std::string& trailing);
    std::string storingName(const std::string& str);
    bool isValidChannelName(const std::string& name);
    bool isMode(char c);
    bool isOtherSign(char oldSign, char newSign);
    bool requireParam(char mode, char sign);

    // Free and cleanup
    void ClearDisconnectedClients();
    void deleteClientData(Client *client);
    bool startsWith(const std::string& str, const std::string& set) {return !str.empty() && set.find(str[0]) != std::string::npos;}
    void deleteUserFromChannels(Client *client);

};

int validateUserLimit(const std::string& param);



#endif