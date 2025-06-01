#include "../Includes/Server.hpp"

void Server::topicMessage(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2)
    {
        respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " TOPIC :Not enough parameters\r\n");
        return;
    }

    std::string channelName = tokens[1];
    if (channelName[0] == '#')
        channelName = storingName(channelName);

    if (!channelExist(channelName))
    {
        respond(client->getClientFd(), ":ircserv 403 " + client->getNickname() + " #" + channelName + " :No such channel\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);
    
    if (!channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 442 " + client->getNickname() + " #" + channelName + " :You're not on that channel\r\n");
        return;
    }

    if (tokens.size() == 2)
    {
        if (channel->getTopic().empty())
            respond(client->getClientFd(), ":ircserv 331 " + client->getNickname() + " #" + channelName + " :No topic is set\r\n");
        else
            respond(client->getClientFd(), ":ircserv 332 " + client->getNickname() + " #" + channelName + " :" + channel->getTopic() + "\r\n");
        return;
    }

    if (channel->isTopicLocked() && !channel->isOperator(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 482 " + client->getNickname() + " #" + channelName + " :You're not channel operator\r\n");
        return;
    }

    std::string topic = "";
    std::string fullTopicStr = "";
    
    for (size_t i = 2; i < tokens.size(); ++i) {
        if (i > 2) fullTopicStr += " ";
        fullTopicStr += tokens[i];
    }
    
    std::vector<std::string> topicTokens = topicSplit(fullTopicStr);
    if (topicTokens.size() > 0)
        topic = topicTokens[0];
    
    channel->setTopic(topic);
    
    std::vector<std::string> members = channel->getUsers();
    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        if (member)
        {
            respond(member->getClientFd(), ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostName() + " TOPIC #" + channelName + " :" + topic + "\r\n");
        }
    }
}