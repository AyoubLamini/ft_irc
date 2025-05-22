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
        std::string _topic;
        
          // Modes
        bool _inviteOnly;             // +i 
        bool _topicLocked;            // +t   
        bool _hasKey;                 // whether +k is set
        size_t  _userLimit;           // +l (0 if no limit)
        bool _hasUserLimit;           // whether +l is set
        size_t _userCount;            // number of users in the channel

        std::vector<std::string> users; // users inculding operators
        std::vector<std::string> operators; // nicknames of +o users


    public :
        Channel(std::string name, std::string key);
        ~Channel();

    
        std::string getName();
        std::string getKey();
        void setName(std::string name);
        void setKey(std::string key);
        void setInviteOnly(bool invite);
        bool isInviteOnly();
        bool isTopicLocked();
        bool isTopicSet();
        void setTopicLocked(bool locked);
        bool isKeySet();
        void setKeySet(bool key);
        void unsetKey();
        bool isUserLimitSet();
        void setUserLimit(size_t limit);
        void setUserLimitSet(bool limit);
        size_t getUserLimit();
        size_t getUserCount();
        void incrementUserCount();
        void decrementUserCount();
        std::string getTopic();
        void setTopic(std::string topic);


        // Channel managment
        void isCorrectKey(std::string key);
        void addOperator(std::string client);
        void deleteOperator(std::string client);
        void addUser(std::string client);
        void deleteUser(std::string client);
        bool isOperator(std::string client);
        bool isUser(std::string client);    
        std::vector <std::string> getUsers();
};


#endif