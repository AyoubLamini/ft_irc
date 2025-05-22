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
        std::string _recvBuffer;


        bool _authenticated;
        bool _registered;
        bool _status; // true == connected, false == disconnected

    public : 
        Client(int client_fd, struct sockaddr_in client_addr);
        ~Client();

        int getClientFd() const;
        struct sockaddr_in getClientAddr() const;
        std::string getUsername() const;
        std::string getNickname() const;
        void setUsername(std::string username);
        void setNickname(std::string nickname);
        bool getStatus() const;
        void setStatus(bool status);
        void setClientFd(int client_fd);
        void setClientAddr(struct sockaddr_in client_addr);

    
        std::string  getSendBuffer();
        std::string  getRecvBuffer();
    
        void appendSendMessage(const std::string& message);
        void appendRecvMessage(const std::string& message);

        void eraseSendMessage(size_t n);
        void eraseRecvMessage();
        void substrRecvMessage(size_t n);

        bool isAuthenticated() const;
        void setAuthenticated(bool auth);
        bool isRegistered() const ;
        void setRegistered(bool reg);

};

#endif