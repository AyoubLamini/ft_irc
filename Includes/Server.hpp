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

// #include "Channel.hpp"
#include "Client.hpp"

class Client;

class Server
{
    private:
        int port;
        int server_fd;
        std::string password;
        std::vector <Client *> _Clients;
        // std::vector <Channel> _Channels;
        std::vector <pollfd> poll_fds;

    public:
    Server();
    ~Server();
    int getPort() const;
    std::string getPassword() const;
    void setPassword(std::string password);
    void setPort(int port);
    int getServerFd() const;
    void setServerFd(int server_fd);

    void initializeServer();
    void run();
    void acceptClient();
    void readClient(int client_fd);

    std::string recvMessage(int client_fd);
    Client *getClientByFd(int client_fd);
    void authenticateClient(Client *client, std::string msg);

    // void createChannel(std::string name, std::string *password);
    // void createChannel(std::string client);
    // void joinChannel(Client *client, std::string channel, std::string *password);

};

#endif