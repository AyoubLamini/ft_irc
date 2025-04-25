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


