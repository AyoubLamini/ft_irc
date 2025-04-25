#include "../Includes/Channel.hpp"


Channel::Channel(std::string name, std::string key)
{
    this->_name = name;
    this->_key = key;
    this->_inviteOnly = false;
    this->_topicLocked = false;
    this->_userLimit = 0;
    this->_hasUserLimit = false;
    if (key.empty())
        this->_hasKey = false;
    else
        this->_hasKey = true;
}

void Channel::isCorrectKey(std::string key)
{
    if (this->_hasKey && this->_key != key)
    {
        std::cout << "Key is incorrect" << std::endl;
        return;
    }
}



