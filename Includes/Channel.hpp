#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Client;

class Channel
{
    private : 
        std::string _name;
        std::string *_password;
        
        std::vector<Client *> _users;
        std::vector<Client *> _operators;
        bool _inviteOnly;

    public :
        Channel(std::string _name, std::string *password);
        void add_user(Client *_client);
        std::string getName() {return this->_name;}
        std::string *getPassword() { return this->_password; }
        void setPassword(std::string *password) { this->_password = password; }
        void setName(std::string name) { this->_name = name; }
        void setInviteOnly(bool invite) { this->_inviteOnly = invite; }
        bool isInviteOnly() { return this->_inviteOnly; }



};




#endif