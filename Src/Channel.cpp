#include "../Includes/Channel.hpp"


Channel::Channel(std::string name, std::string key)
{
    this->_key = key;
    this->_name = name;
    this->_inviteOnly = false;
    this->_topicLocked = false;
    this->_userLimit = 0;
    this->_hasUserLimit = false;
    this->_hasKey = false;
    this->_userCount = 0;
    if (key.empty())
        this->_hasKey = false;
    else
        this->_hasKey = true;
}

Channel::~Channel()
{
    std::cout << "Channel Destructed" << std::endl;
}


// getters / setters

std::string Channel::getName() {return this->_name;}
void Channel::setName(std::string name) { this->_name = name; }

std::string Channel::getKey() { return this->_key;}
void Channel::setKey(std::string key) { this->_key = key; }

void Channel::setInviteOnly(bool invite) { this->_inviteOnly = invite; }
bool Channel::isInviteOnly() { return this->_inviteOnly; }

bool Channel::isTopicLocked() { return this->_topicLocked; }
void Channel::setTopicLocked(bool locked) { this->_topicLocked = locked; }

bool Channel::isKeySet() { return this->_hasKey; }
void Channel::setKeySet(bool key) { this->_hasKey = key; }
void Channel::unsetKey(){this->_key = "";}


bool Channel::isUserLimitSet() { return this->_hasUserLimit; }
void Channel::setUserLimit(size_t limit) { this->_userLimit = limit; }
void Channel::setUserLimitSet(bool limit) { this->_hasUserLimit = limit; }
size_t Channel::getUserLimit() { return this->_userLimit; }

size_t Channel::getUserCount() { return this->_userCount; }
void Channel::incrementUserCount() { this->_userCount++; }
void Channel::decrementUserCount() { this->_userCount--; }

std::string Channel::getTopic() { return this->_topic; };
void Channel::setTopic(std::string topic) { this->_topic = topic; }



// channel managment 

void Channel::isCorrectKey(std::string key)
{
    if (this->_hasKey && this->_key != key)
    {
        std::cout << "Key is incorrect" << std::endl;
        return;
    }
}

void Channel::addUser(std::string client)
{
    this->users.push_back(client);
    this->_userCount++;
}

void Channel::addOperator(std::string client) 
{
    this->operators.push_back(client);
}

void Channel::deleteOperator(std::string client)
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
void Channel::deleteUser(std::string client) 
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
bool Channel::isOperator(std::string client)
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
bool Channel::isUser(std::string client)
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
        
std::vector <std::string> Channel::getUsers() { return this->users; }


