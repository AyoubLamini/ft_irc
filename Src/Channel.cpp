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


