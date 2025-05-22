#include "../Includes/Server.hpp"

void Server::joinMessage(Client *client, const std::vector <std::string>& tokens)
{
    std::vector <std::string> names;
    std::vector <std::string> keys;

    if (tokens.size() < 2)
    {
        respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " JOIN :Not enough parameters\r\n");
        return;
    }
    names = splitedJoin(tokens[1], ',');
    if (tokens.size() > 2)
        keys = splitedJoin(tokens[2], ',');
    if (names.size() + keys.size() > 15)
    {
        respond(client->getClientFd(), ":ircserv 470 " + client->getNickname() + " JOIN :Too many JOIN parameters\r\n");
        return;
    }
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (keys.size() < names.size()) // fill the keys vector with empty strings
            keys.push_back("");
    }
    for (size_t i = 0; i < names.size(); ++i)
    {
        if (!isValidChannelName(names[i]))
        {
            respond(client->getClientFd(), ":ircserv 403 " + client->getNickname() + " " + names[i] + " :Bad channel name\r\n");
        }
        else
        {
            std::string channelName = storingName(names[i]);
            if (channelExist(channelName)) 
            {
                joinChannel(client, channelName, keys[i]);
                std::cout << "Client joined channel" << std::endl;
            }
            else
            {
                if (!keys[i].empty() && !isValidChannelKey(keys[i]))
                    respond(client->getClientFd(), ":ircserv 475 " + client->getNickname() + " " + keys[i] +  " :bad key\r\n");
                else
                {
                    createChannel(client, channelName, keys[i]);
                    std::cout << "Client created channel" << std::endl;
                }
            }

        }
    }
}

void Server::createChannel(Client *client, std::string name, std::string key)
{
    Channel *channel = new Channel(name, key);
    std::cout << "Channel created:|" << channel->getName() << "|"  << std::endl;
    channel->addOperator(client->getNickname());
    channel->addUser(client->getNickname());
    _Channels.push_back(channel);
    respond(client->getClientFd(), formatIrcMessage(client->getNickname(), client->getUsername(), "JOIN", "#" + channel->getName(), ""));
    respond(client->getClientFd(), ":ircserv 331 " + client->getNickname() + " #" + name + " :No topic is set\r\n");
    respond(client->getClientFd(), ":ircserv 353 " + client->getNickname() + " = #" + name + " " + listMembers(channel));
    respond(client->getClientFd(), ":ircserv 366 " + client->getNickname() + " #" + name + " :End of /NAMES list\r\n");
}
std::string Server::listMembers(Channel *channel)
{
    std::vector <std::string> Members = channel->getUsers();
    std::string list = ":";
    for (size_t i = 0; i < Members.size(); i++)
    {
        if (channel->isOperator(Members[i]))
        {
           list += "@";
        }
        list += Members[i] + " ";
    }
    list += "\r\n";
    return list;
}

void Server::joinChannel(Client *client, std::string name, std::string key)
{
    Channel *channel = getChannel(name);
    if (channel->isInviteOnly())
    {
        respond(client->getClientFd(), ":ircserv 473 " + client->getNickname() + " #" + name + " :Cannot join channel (+i) - invite only\r\n");
    }
    else if (channel->isKeySet() && channel->getKey() != key)
    {
        respond(client->getClientFd(), ":ircserv 475 " + client->getNickname() + " #" + name + " :Cannot join channel (+k) - bad key\r\n");
    }
    else if (channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 443 " + client->getNickname() + " #" + name + " :You are already a member of the channel\r\n");
    }
    else if (channel->isUserLimitSet() && channel->getUserLimit() <= channel->getUserCount())
    {
        respond(client->getClientFd(), ":ircserv 471 " + client->getNickname() + " #" + name + " :Cannot join channel (+l) - user limit reached\r\n");
    }
    else
    {
        channel->addUser(client->getNickname());
        sendMessageToChannel(client, channel->getName(), "", "JOIN");
        if (!channel->isTopicSet())
            respond(client->getClientFd(), ":ircserv 331 " + client->getNickname() + " #" + name + " :No topic is set\r\n");
        else
            respond(client->getClientFd(), ":ircserv 332 " + client->getNickname() + " #" + name + " " + channel->getTopic()  + "\r\n");
        respond(client->getClientFd(), ":ircserv 353 " + client->getNickname() + " = #" + name + " " + listMembers(channel));
        respond(client->getClientFd(), ":ircserv 366 " + client->getNickname() + " #" + name + " :End of /NAMES list\r\n");
    }
}