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
    _sendBuffer.clear();
    char *ip_str = inet_ntoa(client_addr.sin_addr);
    _hostname = ip_str;
   std::cout << BOLD << GREEN << "[+] New client connected  " 
          << RESET << "| IP: " << CYAN << _hostname 
          << RESET << " | FD: " << client_fd << std::endl;
}

Client::~Client()
{
    close(client_fd);
    std::cout << BOLD << RED << "[-] Client disconnected   " 
          << RESET << "| IP: " << CYAN << "10.13.2.4" 
          << RESET << " | FD: " << 4 << std::endl;
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
std::string Client::getHostName()
{
    return (this->_hostname);
}


void Client::setClientFd(int client_fd)
{
    this->client_fd = client_fd;
}

void Client::setClientAddr(struct sockaddr_in client_addr)
{
    this->client_addr = client_addr;
}

bool Client::getStatus() const
{ 
    return _status; 
}

void Client::setStatus(bool status) { _status = status; }


std::string  Client::getSendBuffer() {return _sendBuffer;}
std::string  Client::getRecvBuffer() {return _recvBuffer;}

void Client::appendSendMessage(const std::string& message) {_sendBuffer += message;}
void Client::appendRecvMessage(const std::string& message) {_recvBuffer += message;}


void Client::eraseSendMessage(size_t n) { _sendBuffer.erase(0, n);}
void Client::substrRecvMessage(size_t n) {_recvBuffer.substr(0, n);}
void Client::eraseRecvMessage(size_t n) 
{
    _recvBuffer.erase(0, n);
}


bool Client::isAuthenticated() const { return _authenticated; }
void Client::setAuthenticated(bool auth) { _authenticated = auth; }
bool Client::isRegistered() const { return _registered; }
void Client::setRegistered(bool reg) { _registered = reg; }