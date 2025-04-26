#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Client;
class Server;

class Channel
{
    private : 
        std::string _name;
        std::string _key;
        
          // Modes
        bool _inviteOnly;             // +i
        bool _topicLocked;            // +t         // +k (empty string if no key)
        bool _hasKey;                 // whether +k is set
        size_t  _userLimit;               // +l (0 if no limit)
        bool _hasUserLimit;           // whether +l is set
        size_t _userCount;            // number of users in the channel

        std::vector<std::string> users; // users inculding operators
        std::vector<std::string> operators; // nicknames/usernames of +o users


    public :
        Channel(std::string name, std::string key);
        ~Channel();
        std::string getName() {return this->_name;}
        std::string getKey() { return this->_key;}
        void setName(std::string name) { this->_name = name; }
        void setKey(std::string key) { this->_key = key; }
        void setInviteOnly(bool invite) { this->_inviteOnly = invite; }
        bool isInviteOnly() { return this->_inviteOnly; }
        bool isTopicLocked() { return this->_topicLocked; }
        void setTopicLocked(bool locked) { this->_topicLocked = locked; }
        bool isKeySet() { return this->_hasKey; }
        void setKeySet(bool key) { this->_hasKey = key; }
        bool isUserLimitSet() { return this->_hasUserLimit; }
        void setUserLimitSet(bool limit) { this->_hasUserLimit = limit; }
        size_t getUserLimit() { return this->_userLimit; }
        size_t getUserCount() { return this->_userCount; }
        void incrementUserCount() { this->_userCount++; }
        void decrementUserCount() { this->_userCount--; }

        // Channel managment
        void isCorrectKey(std::string key);
        void addOperator(std::string client) {this->operators.push_back(client);}
        void deleteOperator(std::string client) 
        {
            for (size_t i = 0; i < this->operators.size(); ++i)
            {
                if (this->operators[i] == client)
                {
                    this->operators.erase(this->operators.begin() + i);
                    break;
                }
            }
        }
        void addUser(std::string client);
        void deleteUser(std::string client) 
        {
            for (size_t i = 0; i < this->users.size(); ++i)
            {
                if (this->users[i] == client)
                {
                    this->users.erase(this->users.begin() + i);
                    this->decrementUserCount();
                    break;
                }
            }
        }
        bool isOperator(std::string client)
        {
            for (size_t i = 0; i < this->operators.size(); ++i)
            {
                if (this->operators[i] == client)
                {
                    return true;
                }
            }
            return false;
        }
        bool isUser(std::string client)
        {
            for (size_t i = 0; i < this->users.size(); ++i)
            {
                if (this->users[i] == client)
                {
                    return true;
                }
            }
            return false;
        }
        
        std::vector <std::string> getUsers() { return this->users; }


};




#endif

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;

class Client
{
    private :
        struct sockaddr_in client_addr;
        int client_fd;
        std::string _username;
        std::string _nickname;
        std::string _sendBuffer;


        


        bool _authenticated;
        bool _registered;
        bool _status; // true == connected, false == disconnected


        // std::vector <Channel> _Channels;
    public : 
       


        Client(int client_fd, struct sockaddr_in client_addr);
        ~Client();

        // Getters / Setters
        int getClientFd() const;
        struct sockaddr_in getClientAddr() const;
        std::string getUsername() const;
        std::string getNickname() const;
        void setUsername(std::string username);
        void setNickname(std::string nickname);

        bool getStatus() const { return _status; }
        void setStatus(bool status) { _status = status; }

        void setClientFd(int client_fd);
        void setClientAddr(struct sockaddr_in client_addr);
        std::string  getBuffer() {return _sendBuffer;}
         
        void appendMessage(const std::string& message) {_sendBuffer += message;}
        void eraseMessage(size_t n);

        // Authentication And Registration
        bool isAuthenticated() const { return _authenticated; }
        void setAuthenticated(bool auth) { _authenticated = auth; }
        bool isRegistered() const { return _registered; }
        void setRegistered(bool reg) { _registered = reg; }


        

    
};

#endif

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

    void processCommands(Client *client, const std::vector <std::string>& tokens);
    void joinMessage(Client *client, const std::vector <std::string>& tokens);
    

  
    // Channel managment

    bool channelExist(std::string channel);
    Channel *getChannel(std::string channel);
    void createChannel(Client *client, std::string name, std::string key);
    void deleteEpmtyChannels();
    void joinChannel(Client *client, std::string name, std::string key);
    void sendMessageToChannel(Client *sender, std::string channelName, std::string message, const std::string& messageType);








//   utils
    std::vector<std::string> splitedInput(const std::string& input, char delimiter);
    bool inCommandslist(std::string command);
    bool isValidNickname(const std::string& nickname) ;
    void printMessage(const std::vector<std::string>& tokens);
    bool nickExists(std::string nickname);
    std::vector<std::string> splitedJoin(const std::string& input);
    std::string formatIrcMessage(const std::string& prefixNick, const std::string& prefixUser, const std::string& command, const std::string& target, const std::string& trailing);
    std::string storingName(const std::string& str);
    bool isValidChannelName(const std::string& name);

    // Free and cleanup
    void ClearDisconnectedClients();
    void deleteClientData(Client *client);
    bool startsWith(const std::string& str, const std::string& set) {return !str.empty() && set.find(str[0]) != std::string::npos;}
    void deleteUserFromChannels(Client *client);

};



#endif

#include "../Includes/Channel.hpp"


Channel::Channel(std::string name, std::string key)
{
    this->_key = key;
    this->_name = name;
    this->_inviteOnly = false;
    this->_topicLocked = false;
    this->_userLimit = 0;
    this->_hasUserLimit = false;
    this->_hasKey = false;
    this->_userCount = 0;
    if (key.empty())
        this->_hasKey = false;
    else
        this->_hasKey = true;
}

Channel::~Channel()
{
    std::cout << "Channel Destructed" << std::endl;
}

void Channel::isCorrectKey(std::string key)
{
    if (this->_hasKey && this->_key != key)
    {
        std::cout << "Key is incorrect" << std::endl;
        return;
    }
}
void Channel::addUser(std::string client)
{
    this->users.push_back(client);
    this->_userCount++;
}

#include "../Includes/Client.hpp"


Client::Client(int client_fd, struct sockaddr_in client_addr)
{
    this->client_fd = client_fd;
    this->client_addr = client_addr;

    _authenticated = false;
    _registered = false;
    _username = "";
    _nickname = "";
    _status = true;
}

Client::~Client()
{
    close(client_fd);
    std::cout << "Client Destructed" << std::endl;
}

int Client::getClientFd() const
{
    return (this->client_fd);
}

struct sockaddr_in Client::getClientAddr() const
{
    return (this->client_addr);
}
std::string Client::getUsername() const
{
    return (this->_username);
}
std::string Client::getNickname() const
{
    return (this->_nickname);
}
void Client::setUsername(std::string username)
{
    this->_username = username;
}
void Client::setNickname(std::string nickname)
{
    this->_nickname = nickname;
}

void Client::setClientFd(int client_fd)
{
    this->client_fd = client_fd;
}

void Client::setClientAddr(struct sockaddr_in client_addr)
{
    this->client_addr = client_addr;
}


void Client::eraseMessage(size_t n)
{ 
    _sendBuffer.erase(0, n);
}


#include "../Includes/Server.hpp"

Server::Server()
{
    server_fd = -1;
}

Server::~Server()
{
    for (size_t i = 0; i < poll_fds.size(); ++i) 
    {
        close(poll_fds[i].fd);
    }
    if (server_fd != -1) 
    {
        close(server_fd);
    }
}

std::string Server::getPassword() const
{
    return (this->password);
}
int Server::getPort() const
{
    return (this->port);
}

void Server::setPassword(std::string password) 
{
    this->password = password;
}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setServerFd(int server_fd)
{
    this->server_fd = server_fd;
}
int Server::getServerFd() const
{
    return (this->server_fd);
}

void Server::initializeServer()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // creating TCP socket
    if (server_fd < 0) {
        perror("socket");
        return;
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Set socket options to reuse the address

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the struct

    server_addr.sin_family = AF_INET; // Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available IP addresses
    server_addr.sin_port = htons(port); // Convert the port to network byte order

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        close(server_fd);
        return;
    }

    struct pollfd pfd;
    pfd.fd = server_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    poll_fds.push_back(pfd);
}


void Server::run()
{
    while (true) 
    {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) 
        {
            perror("poll");
            break;
        }
        for (size_t i = 0; i < poll_fds.size(); ++i)
        {
            if (poll_fds[i].revents & POLLIN) // if true => there is data to read
            {
                if (poll_fds[i].fd == server_fd)  // if true, means its server socket, means its a new connection
                {
                    acceptClient();
                } 
                else 
                {
                    readClient(poll_fds[i].fd);
                }
            }
            if (poll_fds[i].revents & POLLOUT) 
            {
                writeClient(poll_fds[i].fd); 
            }
        }
        ClearDisconnectedClients();
    }
}

void Server::ClearDisconnectedClients() 
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getStatus() == false && _Clients[i]->getBuffer().empty()) 
        {
            deleteUserFromChannels(_Clients[i]);
            deleteEpmtyChannels();
            deleteClientData(_Clients[i]);
            _Clients.erase(_Clients.begin() + i);
            --i;
        }
    }
}

void Server::deleteEpmtyChannels()
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        if (_Channels[i]->getUserCount() == 0) 
        {
            delete _Channels[i];
            _Channels.erase(_Channels.begin() + i);
            --i;
        }
    }
}


void Server::respond(int client_fd, const std::string& message)
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL)
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    client->appendMessage(message);
    for (size_t i = 0; i < poll_fds.size(); ++i) 
    {
        if (poll_fds[i].fd == client_fd) 
        {
            poll_fds[i].events |= POLLOUT;
            break;
        }
    }
}

void Server::writeClient(int client_fd)
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL)
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    std::string message = client->getBuffer();
    size_t bytes_sent = send(client_fd, message.c_str(), message.length(), 0);
    if (bytes_sent < 0) 
    {
        perror("send");
        close(client_fd);
        return;
    }
    client->eraseMessage(bytes_sent);
    if (client->getBuffer().empty())
    {
        for (size_t i = 0; i < poll_fds.size(); ++i) 
        {
            if (poll_fds[i].fd == client_fd) {
                poll_fds[i].events &= ~POLLOUT;
            break;
        }
    }
}
}


void Server::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        return;
    }
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags == -1 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl");
        close(client_fd);
        return;
    }

    std::cout << "Client connected" << std::endl;

    // Add the new client to the poll_fds vector
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    poll_fds.push_back(pfd);
    // create a new Client object and add it to the _Clients vector
    Client *new_client = new Client(client_fd, client_addr);
    _Clients.push_back(new_client);
}






void Server::readClient(int client_fd) // Read client socket
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL) 
    {
        std::cout << "Client not found" << std::endl;
        return;
    }

    std::string msg = recvMessage(client_fd);
    std::vector<std::string> tokens = splitedInput(msg, ' ');
    if (!tokens.empty())
    {
        // Authentication and Registration
        if (!client->isAuthenticated() || !client->isRegistered()) 
        {
            if (!client->isAuthenticated())
                authenticateClient(client, tokens);
            else
            {
                registerClient(client, tokens);
                checkRegistration(client);
            }
        }
        // Commands
        else
        {
            if (tokens[0] == "PASS" || tokens[0] == "NICK" || tokens[0] == "USER")
            {
                respond(client->getClientFd(), ":ircserv 462 * :Already registered\r\n");
                return; 
            }
            else if (inCommandslist(tokens[0]))
            {
                processCommands(client, tokens);
            }
            else
            {
                respond(client->getClientFd(), ":ircserv 421 * :Unknown command\r\n");
                return; 
            }
        }
    }
    else
        std::cout << "Tokens empty" << std::endl;
}



std::string Server::recvMessage(int client_fd) // Recive Message
{
    char buffer[1024];
    Client *client = getClientByFd(client_fd);
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) 
    {
        if (bytes_read == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            client->setStatus(false);
        }
        else 
        {
            perror("recv");
        }
        return std::string(); // Return empty string on error
    }
    buffer[bytes_read] = '\0';
    std::string msg =  std::string(buffer);
    if (msg.back() == '\n')
        msg.pop_back();
    if (msg.back() == '\r')
        msg.pop_back();
    return msg;
}

    
void Server::authenticateClient(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens[0] == "PASS")
    {
        if (tokens.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 461 * :Not enough parameters\r\n");
            client->setStatus(false);
            return;
        }
        else if (tokens[1] == this->password)
        {
            client->setAuthenticated(true);
            std::cout << "Client authenticated" << std::endl;
            return;
        }
        else
        {
            respond(client->getClientFd(), ":ircserv 464 * :Password incorrect\r\n");
            client->setStatus(false);
            return;
        }
    }
    else if (inCommandslist(tokens[0]))
    {
        respond(client->getClientFd(), ":ircserv 451 * :You have not registered\r\n");
        client->setStatus(false);
        return;
    }
    else
    {
        respond(client->getClientFd(), ":ircserv 421 * :Unknown command\r\n");
        client->setStatus(false);
        return;
    }
}






void Server::registerClient(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens[0] == "NICK" && client->getNickname().empty()) // Nickname Validation
    {
        if (tokens.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 431 * :No nickname given\r\n");
            return;
        }
        else if (nickExists(tokens[1]))
        {
            respond(client->getClientFd(), ":ircserv 433 * " + tokens[1] + " :Nickname is already in use\r\n");
            return;
        }
        else if(!isValidNickname(tokens[1]))
        {
            respond(client->getClientFd(), ":ircserv 432 * " + tokens[1] + " :Erroneous nickname\r\n");
            return;
        }
        else
        {
            client->setNickname(tokens[1]);
            std::cout << "Nickname entered" << std::endl;
            return;
        }
      
    }
    else if (tokens[0] == "USER" && client->getUsername().empty()) // USERNAME VALIDATION
    {
        if (tokens.size() < 5)
        {
            respond(client->getClientFd(), ":ircserv 461 USER * :Not enough parameters\r\n");
            return;
        }
        else if (tokens.size() > 15)
        {
            respond(client->getClientFd(), ":ircserv 461 USER * :Too many parameters\r\n");
            return;
        }
        else
        {
            client->setUsername(tokens[1]);
            std::cout << "username entered" << std::endl;
            return;
        }

    }
    else if (inCommandslist(tokens[0]))
    {
        respond(client->getClientFd(), ":ircserv 451 * :You have not registered\r\n");
        return;
    }
    else
    {
        respond(client->getClientFd(), ":ircserv 421 * :Unknown command\r\n");
        return;
    }
}

void Server::checkRegistration(Client *client)
{
    if (client->getUsername() != "" && client->getNickname() != "")
    {
        client->setRegistered(true);
        respond(client->getClientFd(), ":ircserv 001 " + client->getNickname() + " :Welcome to the IRC server\r\n");
        std::cout << "Client registered" << std::endl;
    }
}


void Server::processCommands(Client *client, const std::vector <std::string>& tokens)
{
    if (tokens[0] == "JOIN")
    {
        joinMessage(client, tokens);
    }
}

void Server::joinMessage(Client *client, const std::vector <std::string>& tokens)
{
    std::vector <std::string> names;
    std::vector <std::string> keys;

    if (tokens.size() < 2)
    {
        respond(client->getClientFd(), ":ircserv 461 JOIN * :Not enough parameters\r\n");
        return;
    }

    names = splitedInput(tokens[1], ',');
    if (tokens.size() > 2)
        keys = splitedInput(tokens[2], ',');
    if (names.size() + keys.size() > 15)
    {
        respond(client->getClientFd(), ":ircserv 470 * :Too many JOIN parameters\r\n");
        return;
    }
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (keys.size() < names.size()) // fill the keys vector with empty strings
            keys.push_back("");
    }
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (!isValidChannelName(names[i]))
        {
            respond(client->getClientFd(), ":ircserv 403 * :" + names[i] + " :Bad channel name\r\n");
        }
        else
        {
            std::string channelName = storingName(names[i]);
            if (channelExist(channelName)) 
            {
                joinChannel(client, channelName, keys[i]);
                std::cout << "Client joined channel" << std::endl;
            }
            else
            {
                createChannel(client, channelName, keys[i]);
                std::cout << "Client created channel" << std::endl;
            }
        }
    }
}

// Channel managment 

bool Server::channelExist(std::string channel)
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        if (_Channels[i]->getName() == channel) 
        {
            return true;
        }
    }
    return false;
}

Channel *Server::getChannel(std::string channel)
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        if (_Channels[i]->getName() == channel) 
        {
            return _Channels[i];
        }
    }
    return NULL;
}

void Server::createChannel(Client *client, std::string name, std::string key)
{
    Channel *channel = new Channel(name, key);
    std::cout << "Channel created:|" << channel->getName() << "|"  << std::endl;
    channel->addOperator(client->getNickname());
    channel->addUser(client->getNickname());
    _Channels.push_back(channel);
    respond(client->getClientFd(), formatIrcMessage(client->getNickname(), client->getUsername(), "JOIN", " #" + channel->getName(), ""));
}

void Server::joinChannel(Client *client, std::string name, std::string key)
{
    Channel *channel = getChannel(name);
    if (channel->isKeySet() && channel->getKey() != key)
    {
        respond(client->getClientFd(), ":ircserv 475 * : #" + name + " :Cannot join channel (+k) - bad key\r\n");
    }
    else if (channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 443 * : #" + name + " :You are already a member of the channel\r\n");
    }
    else if (channel->isInviteOnly())
    {
        respond(client->getClientFd(), ":ircserv 473 * : #" + name + " :Cannot join channel (+i) - invite only\r\n");
    }
    else if (channel->isUserLimitSet() && channel->getUserLimit() <= channel->getUserCount())
    {
        respond(client->getClientFd(), ":ircserv 471 * : #" + name + " :Cannot join channel (+l) - user limit reached\r\n");
    }
    else
    {
        channel->addUser(client->getNickname());
        sendMessageToChannel(client, channel->getName(), "", "JOIN");
    }

}




// -----------------------------------------------------------------------------------------------
void Server::sendMessageToChannel(Client *sender, std::string channelName, std::string message, const std::string& messageType)
{
    // print channels
    std::cout << "Channels: " << std::endl;
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        std::cout << "Channel[" << i << "] => " << _Channels[i]->getName() << std::endl;
    }
    Channel *channel = getChannel(channelName);
    if (channel == NULL)
    {
        std::cout << "NO SUCH CHANNEL: " << channelName << std::endl;
        respond(sender->getClientFd(), ":ircserv 403 * :" + channelName + " :No such channel\r\n");
        return;
    }
    std::vector <std::string> members = channel->getUsers();
    std::cout << "Members size: " << members.size() << std::endl;

    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        std::string reply;
        // exit(0);
        // Check message type and format accordingly
        if (messageType == "PRIVMSG") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PRIVMSG", " #" + channelName, message);
        } else if (messageType == "JOIN") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "JOIN", " #" + channelName, "");
        } else if (messageType == "PART") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PART", " #" + channelName, "client disconnected");
        }
        respond(member->getClientFd(), reply);
    }
}

bool Server::nickExists(std::string nickname)
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getNickname() == nickname) 
        {
            return true;
        }
    }
    return false;
}

Client *Server::getClientByFd(int client_fd)
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getClientFd() == client_fd) 
        {
            return _Clients[i];
        }
    }
    return NULL;
}

Client *Server::getClientByNickname(const std::string& nickname)
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getNickname() == nickname) 
        {
            return _Clients[i];
        }
    }
    return NULL;
}

void Server::deleteUserFromChannels(Client *client)
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        Channel *channel = _Channels[i];
        if (channel->isUser(client->getNickname()))
        {
            sendMessageToChannel(client, channel->getName(), "", "PART");
            channel->deleteUser(client->getNickname());
        }
    }
}

void Server::deleteClientData(Client *client)
{
    for (size_t i = 0; i < this->poll_fds.size(); i++)
    {
        if (poll_fds[i].fd == client->getClientFd())
        {
            poll_fds.erase(poll_fds.begin() + i);
            break;
        }
    }
    delete client;
}



// Helper Functions ------------------------------------------------------------------------------------------------


std::vector<std::string> Server::splitedInput(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(input);

    while (std::getline(iss, token, delimiter))
    {
        // Remove leading spaces
        size_t start = 0;
        while (start < token.size() && (token[start] == ' ' || token[start] == '\t')) {
            ++start;
        }
        token = token.substr(start);

        // Remove trailing spaces
        size_t end = token.size();
        while (end > start && (token[end - 1] == ' ' || token[end - 1] == '\t')) {
            --end;
        }
        token = token.substr(0, end);

        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool Server::inCommandslist(std::string command)
{

    if (
        command == "NICK" 
        || command == "USER"
        || command ==  "PASS"
        || command == "PONG"
        || command == "PING"
        || command == "JOIN"
        || command == "PART"
        || command == "PRIVMSG"
        || command == "NOTICE"
        || command == "TOPIC"
        || command == "MODE"
        || command == "KICK"
        || command == "INVITE"
        || command == "WHOIS"
    )
    {
        return true;
    }
    return false;
    
}


bool Server::isValidNickname(const std::string& nickname)
{
    if (nickname.empty() || nickname.length() > 9)
        return false;

    // First character must be a letter or special
    if (!isalpha(nickname[0]) && (nickname[0] < '[' || nickname[0] > '`') && (nickname[0] < '{' || nickname[0] > '}'))
        return false;

    // Remaining characters: letter, digit, special, or '-'
    for (size_t i = 1; i < nickname.length(); ++i)
    {
        if (!isalnum(nickname[i]) && 
            (nickname[i] < '[' || nickname[i] > '`') && 
            (nickname[i] < '{' || nickname[i] > '}') && 
            nickname[i] != '-')
            return false;
    }
    return true;
}


bool Server::isValidChannelName(const std::string& name)
{
    if (name.empty())
        return false;

    // Must start with &, #, +, or !
    if (name[0] != '&' && name[0] != '#' && name[0] != '+' && name[0] != '!')
        return false;

    // Length between 4 and 50 characters
    if (name.length() < 4 || name.length() > 50)
        return false;

    // Cannot contain space, comma, or ASCII 7 (BEL)
    for (size_t i = 0; i < name.length(); ++i)
    {
        if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
            return false;
    }
    return true;
}

void Server::printMessage(const std::vector<std::string>& tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "token[" << i << "]=> ";
        std::cout << "\'" << tokens[i] << "\'" << std::endl;
    }
}


std::string Server::formatIrcMessage(const std::string& prefixNick, const std::string& prefixUser, const std::string& command, const std::string& target, const std::string& trailing)
{
    std::string message = ":" + prefixNick + "!" + prefixUser + "@host " + command + " " + target;
    if (!trailing.empty())
        message += " :" + trailing;
    message += "\r\n";
    return message;
}


std::string Server::storingName(const std::string& str)
{
    std::string result = str;
    result = result.substr(1);
    for (size_t i = 0; i < result.length(); ++i)
        result[i] = std::toupper(result[i]);
    return result;
}

#include "../Includes/Server.hpp"

void SetPortandPassword(char **argv, Server &Server)
{
    if (argv[2] == NULL)
    {
        std::cout << "No password provided" << std::endl;
        exit(1);
    }
    else
    {
        std::string password = argv[2];
        if (password.empty())
        {
            std::cout << "No password provided" << std::endl;
            exit(1);
        }
        else if (password.length() < 8)
        {
            std::cout << "Password too short" << std::endl;
            exit(1);
        }
        Server.setPassword(password);
    }
    if (argv[1] == NULL)
    {
        std::cout << "No port provided" << std::endl;
        exit(1);
    }
    else
    {
        int port = atoi(argv[1]);
        if (port < 1024 || port > 65535)
        {
            std::cout << "Out of range ( 1024 - 65535 ) try again !! " << std::endl;
            exit(1);
        }
        Server.setPort(port);
        std::cout << "Port set to " << port << std::endl;
    }
}

int main(int argc, char **argv) {

    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    Server Server;
    SetPortandPassword(argv, Server);
    Server.initializeServer();
    Server.run();
   
    return 0;
}
