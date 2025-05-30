#include "../Includes/Server.hpp"
#include <errno.h>

int is_fd_open(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

Server::Server()
{
    server_fd = -1;
}

Server::~Server()
{
    if (server_fd != -1) 
    {
        close(server_fd);
    }
    std::cout << "closing: " << server_fd << std::endl;
    std::cout << "Server destructed" << std::endl;
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

bool Server::Signal = false;

void Server::SignalHandler(int signum)
{
    (void)signum;
    Server::Signal = true;
}

void Server::initializeServer()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // creating TCP socket
    if (server_fd < 0) {
        perror("socket");
        cleanAndExit();
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Set socket options to reuse the address

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the struct

    server_addr.sin_family = AF_INET; // Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available IP addresses // network interfaces 
    server_addr.sin_port = htons(port); // Convert the port to network byte order 

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        cleanAndExit();
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        cleanAndExit();
        return;
    }
    printBanner(port);

    if (fcntl (server_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl");
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
    while (Server::Signal == false) 
    {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0 && Server::Signal == false) 
        {
            perror("poll");
            break;
        }
        for (size_t i = 0; i < poll_fds.size(); ++i)
        {
            if (Server::Signal == true)
                break;
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

            if (poll_fds[i].revents & POLLHUP)
            {
                disconnectClient(poll_fds[i].fd); 
            }
            if (poll_fds[i].revents & POLLOUT)  // data can be sent through the socket
            {
                writeClient(poll_fds[i].fd); 
            }
        }
        ClearDisconnectedClients();
    }
    cleanAndExit();
}


void Server::disconnectClient(int client_fd)
{
    writeClient(client_fd); 
}

void Server::ClearDisconnectedClients() 
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getStatus() == false && _Clients[i]->getSendBuffer().empty()) 
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

void Server::cleanAndExit()
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
            deleteClientData(_Clients[i]);
            _Clients.erase(_Clients.begin() + i);
            --i;
    }
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
            delete _Channels[i];
            _Channels.erase(_Channels.begin() + i);
            --i;
    }
    close(server_fd);
    std::cout << "Server is closed" << std::endl;
    exit(0);
}


void Server::respond(int client_fd, const std::string& message)
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL)
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    client->appendSendMessage(message);
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
    std::string message = client->getSendBuffer();
    size_t bytes_sent = send(client_fd, message.c_str(), message.length(), 0);
    if (bytes_sent < 0) 
    {
        perror("send");
        close(client_fd);
        return;
    }
    client->eraseSendMessage(bytes_sent);
    if (client->getSendBuffer().empty())
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


    // Need to revise this part


    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        return;
    }


    if (fcntl (client_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        perror("fcntl");
        close(client_fd);
        return;
    }

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



std::string Server::getCommandLine(Client *client)
{
    std::string line = "";
    size_t pos;
    pos = newLinePosition(client->getRecvBuffer());
    if (pos != std::string::npos)
    {
        newLineOnly(client->getRecvBuffer()) ? pos += 1 : pos += 2; 
        line = client->getRecvBuffer().substr(0, pos);
        client->eraseRecvMessage(pos);
    }
    return line;
}

std::string removeNewLine(std::string line)
{
    if (line.find('\n') != std::string::npos)
        line.pop_back();
    if (line.find('\r') != std::string::npos)
        line.pop_back();
    return line;
}

void Server::readClient(int client_fd) // Read client socket
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL) 
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    bool recived = recvMessage(client_fd);
    if (!recived)
    {
        return;
    }
    std::string line = getCommandLine(client);
    while (recived && has_newline(line))
    {
        line = removeNewLine(line);
        std::cout << "Line : |" << line << std::endl;
        if (line.empty())
            return;
        std::vector<std::string> tokens = splitedInput(line, ' ');
        
        // Authentication and Registration
        if (!client->isAuthenticated() || !client->isRegistered()) 
        {
            if (!client->isAuthenticated())
            {
                if  (!authenticateClient(client, tokens))
                    break;
            }
            else
            {
                registerClient(client, tokens);
                checkRegistration(client);
            }
        }
        else 
        // registered client commands processing
        {
            if (tokens[0] == "PASS" || tokens[0] == "NICK" || tokens[0] == "USER")
            {
                respond(client->getClientFd(), ":ircserv 462 " + client->getNickname() + " :Already registered\r\n");
                return; 
            }
            else if (has_non_printables(line))
            {
                respond(client->getClientFd(), ":ircserv 421 " + client->getNickname() + ":Command contains non-printable characters\r\n");
                return; 
            }
            else if (inCommandslist(tokens[0])) 
            {
                processCommands(client, tokens, line);
            }
            else
            {
                respond(client->getClientFd(), ":ircserv 421 " + client->getNickname() + " :Unknown command\r\n");
                return; 
            }
        }
        line = getCommandLine(client);
    }

}

bool Server::recvMessage(int client_fd) // Recive Message
{
    char buffer[1024];
    Client *client = getClientByFd(client_fd);
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) 
    {
        if (bytes_read == 0)
        {
            client->setStatus(false);
        }
        else
        {
            perror("recv");
        }
       return false;
    }
    buffer[bytes_read] = '\0';
    client->appendRecvMessage(buffer);
    return true;
}

void Server::processCommands(Client *client, const std::vector <std::string>& tokens, std::string msg)
{
    if (tokens[0] == "JOIN")
    {
        joinMessage(client, tokens);
    }
    else if (tokens[0] == "PRIVMSG")
    {
        privateMessage(client, msg);
    }
    else if (tokens[0] == "INVITE")
    {
        inviteToChannel(client, tokens);
    }
    else if (tokens[0] == "MODE")
    {
        channelMode(client, tokens);
    }
    else if (tokens[0] == "TOPIC")
    {
        topicMessage(client, tokens);
    }
    else if (tokens[0] == "KICK")
    {
        kickMessage(client, tokens);
    }
    else if (tokens[0] == "QUIT")
    {
       client->setStatus(false);
    }
}



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
