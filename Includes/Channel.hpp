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
        std::string _topic;

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

        std::string getTopic() { return this->_topic; }
        void setTopic(std::string topic) { this->_topic = topic; }
};




#endif