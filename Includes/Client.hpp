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
        bool _authenticated;
        bool _registered;
        // std::vector <Channel> _Channels;
    public : 
       


        Client(int client_fd, struct sockaddr_in client_addr);
        ~Client();
        int getClientFd() const;
        struct sockaddr_in getClientAddr() const;
        std::string getUsername() const;
        std::string getNickname() const;
        void setUsername(std::string username);
        void setNickname(std::string nickname);
        void setClientFd(int client_fd);
        void setClientAddr(struct sockaddr_in client_addr);


        // Authentication And Registration
        bool isAuthenticated() const { return _authenticated; }
        void setAuthenticated(bool auth) { _authenticated = auth; }
        bool isRegistered() const { return _registered; }
        void setRegistered(bool reg) { _registered = reg; }


        

    
};

#endif