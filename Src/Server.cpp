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


void Server::readClient(int client_fd)
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL) 
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    std::string msg = recvMessage(client_fd);
    if (!client->isAuthenticated())
    {
        authenticateClient(client, msg);
    }




    // Handle the received data here
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

std::string Server::recvMessage(int client_fd)
{
    char buffer[1024];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) 
    {
        if (bytes_read == 0) // 
        {
            std::cout << "Client disconnected" << std::endl;
            // Remove the client from the poll_fds vector
            for (size_t i = 0; i < poll_fds.size(); ++i) 
            {
                if (poll_fds[i].fd == client_fd) 
                {
                    poll_fds.erase(poll_fds.begin() + i);
                    break;
                }
            }
        } 
        else 
        {
            perror("recv");
        }
        close(client_fd);
        return std::string(); // Return empty string on error
    }
    buffer[bytes_read] = '\0';
    std::cout << "Received: " << buffer;
    std::string msg =  std::string(buffer);
    if (msg.back() == '\n')
        msg.pop_back();
    if (msg.back() == '\r')
        msg.pop_back();
    return msg;
}

void Server::authenticateClient(Client *client, std::string msg)
{
    if (msg.substr(0, 5) == "PASS ")
    {
        std::string client_pass = msg.substr(5);
        if (client_pass == this->password)
        {
            client->setAuthenticated(true);
            const char *welcome = "Welcome to the server!\n";
            send(client->getClientFd(), welcome, strlen(welcome), 0);
            return;
        }
        else
        {
            std::string reply = ":ircserv 464 * :Password incorrect\r\n";
            send(client->getClientFd(), reply.c_str(), reply.length(), 0);
            close(client->getClientFd());
        }
    }
    else 
    {
        std::string reply = ":ircserv 462 * :You may not reregister\r\n";
        send(client->getClientFd(), reply.c_str(), reply.length(), 0);
        close(client->getClientFd());
    }
    return;
}







