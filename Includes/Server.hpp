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
#include <csignal> 

#include "Channel.hpp"
#include "Client.hpp"

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"

class Client;

class Channel;

class Server
{
    private:
        int port;
        int server_fd;
        std::string password;
        static bool Signal;
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



   // Setup and Server managment 
    void initializeServer();
    void run();
    void acceptClient();
    void readClient(int client_fd);
    bool recvMessage(int client_fd);
    void respond(int client_fd, const std::string& message);
    void writeClient(int client_fd);

    // Authentication / Registration
    void authenticateClient(Client *client, const std::vector<std::string>& tokens);
    void registerClient(Client *client, const std::vector<std::string>& tokens);
    void checkRegistration(Client *client);


    // Proccess Commands / Messages
    std::string getCommandLine(Client *client);
    void processCommands(Client *client, const std::vector <std::string>& tokens, std::string msg);
    void joinMessage(Client *client, const std::vector <std::string>& tokens);
    void privateMessage(Client *client, std::string msg);
    void inviteToChannel(Client *client, const std::vector <std::string>& tokens);
    void channelMode(Client *client, const std::vector <std::string> &tokens);
    int validateModes(Client *client, Channel *channel, std::string modes, const std::vector <std::string> &tokens);
    int executeMode(Client *client, Channel *channel, char mode, char sign, const std::vector <std::string> &tokens, size_t *counter);
    void joinChannel(Client *client, std::string name, std::string key);
    void topicMessage(Client *client, const std::vector <std::string>& tokens);
    void kickMessage(Client *client, const std::vector<std::string>& tokens);
    

  
    // Client and channel managment

    Client *getClientByFd(int client_fd);
    Client *getClientByNickname(const std::string& nickname);
    Channel *getChannel(std::string channel);
    void sendMessageToClient(Client *sender, std::string targetName, std::string msg);
    void sendMessageToChannel(Client *sender, std::string channelName, std::string message, const std::string& messageType);
    bool nickExists(std::string nickname);
    bool channelExist(std::string channel);
    void createChannel(Client *client, std::string name, std::string key);
    std::string listMembers(Channel *channel);
    void listChannelModes(Client *client, Channel *channel);

    
    

    // clear data
    static void SignalHandler(int signum);
    void ClearDisconnectedClients();
    void deleteClientData(Client *client);
    void deleteUserFromChannels(Client *client);
    void deleteEpmtyChannels();
    void cleanAndExit();

};


// Utils outside the class
int validateUserLimit(const std::string& param);
std::vector<std::string> splitedInput(const std::string& input, char delimiter);
std::vector<std::string> topicSplit(const std::string& input);
bool inCommandslist(std::string command);
bool isValidNickname(const std::string& nickname) ;
bool isValidChannelKey(const std::string& key);
void printMessage(const std::vector<std::string>& tokens);

std::string formatIrcMessage(const std::string& prefixNick, const std::string& prefixUser, const std::string& command, const std::string& target, const std::string& trailing);
std::string storingName(const std::string& str);
bool isValidChannelName(const std::string& name);
bool isMode(char c);
bool isOtherSign(char oldSign, char newSign);
bool requireParam(char mode, char sign);
bool has_non_printables(const std::string& input);
std::vector<std::string> splitedJoin (const std::string& input, char delimiter);
bool startsWith(const std::string& str, const std::string& set);
bool has_newline(const std::string& s);
size_t newLinePosition(const std::string& buffer);
bool hasCR(const std::string& s);
std::vector<std::string> mySplit(const std::string& str, char delimiter) ;
void printBanner(int port);

#endif