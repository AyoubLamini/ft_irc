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
            deleteClientData(_Clients[i]);
            _Clients.erase(_Clients.begin() + i);
            --i;
        }
    }
}


void Server::prepareSendBuffer(int client_fd, const std::string& message)
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
            poll_fds[i].events |= POLLOUT; // Set the socket to be writable
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
    std::vector<std::string> tokens = splitedInput(msg);
    if (!tokens.empty())
    {
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

        else
        {        
            std::cout << "TODO: HANDLE COMMANDS" << std::endl;
            if (tokens[0] == "PASS" || tokens[0] == "NICK" || tokens[0] == "USER")
            {
                std::string reply = ":ircserv 462 * :Already registered\r\n";
                prepareSendBuffer(client->getClientFd(), reply);
                return; 
            }
            else if (inCommandslist(tokens[0])) // in Command List: Join #channel 
            {
                // Handle commands Here
                processCommands(client, tokens);
            }
            else
            {
                std::string reply = ":ircserv 421 * :Unknown command\r\n";
                prepareSendBuffer(client->getClientFd(), reply);
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
        if (bytes_read == 0) // 
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

    
void Server::authenticateClient(Client *client, const std::vector<std::string>& tokens) // 
{
    if (tokens[0] == "PASS")
    {
        if (tokens.size() < 2)
        {
            std::string reply = ":ircserv 461 * :Not enough parameters\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
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
            std::string reply = ":ircserv 464 * :Password incorrect\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
            client->setStatus(false);
            return;
        }

    }
    else if (inCommandslist(tokens[0]))
    {
        std::string reply = ":ircserv 451 * :You have not registered\r\n";
        prepareSendBuffer(client->getClientFd(), reply);
        client->setStatus(false);
        return;
    }
    else
    {
        std::string reply = ":ircserv 421 * :Unknown command\r\n";
        prepareSendBuffer(client->getClientFd(), reply);
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
            std::string reply = ":ircserv 431 * :No nickname given\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
            return;
        }
        else if (nickExists(tokens[1]))
        {
            std::string reply = ":ircserv 433 * " + tokens[1] + " :Nickname is already in use\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
            return;
        }
        else if(!isValidNickname(tokens[1]))
        {
            std::string reply = ":ircserv 432 * " + tokens[1] + " :Erroneous nickname\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
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
            std::string reply = ":ircserv 461 USER * :Not enough parameters\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
            return;
        }
        else if (tokens.size() > 5)
        {
            std::string reply = ":ircserv 461 USER * :Too many parameters\r\n";
            prepareSendBuffer(client->getClientFd(), reply);
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
        std::string reply = ":ircserv 451 * :You have not registered\r\n";
        prepareSendBuffer(client->getClientFd(), reply);
        return;
    }
    else
    {
        std::string reply = ":ircserv 421 * :Unknown command\r\n";
        prepareSendBuffer(client->getClientFd(), reply);
        return;
    }
}

void Server::checkRegistration(Client *client)
{
    if (client->getUsername() != "" && client->getNickname() != "")
    {
        client->setRegistered(true);
        std::string reply = ":ircserv 001 " + client->getNickname() + " :Welcome to the IRC server\r\n";
        prepareSendBuffer(client->getClientFd(), reply);
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
    if (tokens.size() < 2)
    {
        std::string reply = ":ircserv 461 JOIN * :Not enough parameters\r\n";
        prepareSendBuffer(client->getClientFd(), reply);
        return;
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

























std::vector<std::string> Server::splitedInput(const std::string& input)
{
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token)
    {
        if (!token.empty() && token[0] == ':') 
        {
            std::string trailing = token.substr(1);
            std::string rest;
            std::getline(iss, rest);
            if (!rest.empty()) 
            {
                trailing += " " + rest;
            }
            tokens.push_back(trailing);
            break;
        }
        tokens.push_back(token);
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

    const std::string specialStart = "[]\\`_^{}|";
    const std::string allowed = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{}|";

    char first = nickname[0];
    if (!std::isalpha(first) && specialStart.find(first) == std::string::npos)
        return false;

    for (size_t i = 1; i < nickname.length(); ++i) {
        if (allowed.find(nickname[i]) == std::string::npos)
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





