#include "../Includes/Server.hpp"

void Server::topicMessage(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2)
    {
        // Include client nickname in the error message
        respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " TOPIC :Not enough parameters\r\n");
        return;
    }

    std::string channelName = tokens[1];
    // Remove # if present
    if (channelName[0] == '#')
        channelName = storingName(channelName);

    if (!channelExist(channelName))
    {
        // Use correct numeric format for ERR_NOSUCHCHANNEL
        respond(client->getClientFd(), ":ircserv 403 " + client->getNickname() + " #" + channelName + " :No such channel\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);
    
    // Check if user is in the channel
    if (!channel->isUser(client->getNickname()))
    {
        // Use correct numeric format for ERR_NOTONCHANNEL
        respond(client->getClientFd(), ":ircserv 442 " + client->getNickname() + " #" + channelName + " :You're not on that channel\r\n");
        return;
    }

    // If no topic provided, return the current topic
    if (tokens.size() == 2)
    {
        if (channel->getTopic().empty())
            respond(client->getClientFd(), ":ircserv 331 " + client->getNickname() + " #" + channelName + " :No topic is set\r\n");
        else
            respond(client->getClientFd(), ":ircserv 332 " + client->getNickname() + " #" + channelName + " :" + channel->getTopic() + "\r\n");
        return;
    }

    // Changing the topic
    // Check if channel is topic locked and user is not an operator
    if (channel->isTopicLocked() && !channel->isOperator(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 482 " + client->getNickname() + " #" + channelName + " :You're not channel operator\r\n");
        return;
    }

    // Get the topic from the tokens (starting from token 2)
    std::string topic = "";
    std::string fullTopicStr = "";
    
    // Reconstruct the full topic string from tokens[2] onward
    for (size_t i = 2; i < tokens.size(); ++i) {
        if (i > 2) fullTopicStr += " ";
        fullTopicStr += tokens[i];
    }
    
    // Use your topicSplit function to handle the topic with potential colon
    std::vector<std::string> topicTokens = topicSplit(fullTopicStr);
    if (topicTokens.size() > 0)
        topic = topicTokens[0];
    
    // Set the topic
    channel->setTopic(topic);
    
    // Notify all users in the channel of the topic change
    std::vector<std::string> members = channel->getUsers();
    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        if (member)
        {
            // Use client's actual hostname instead of fixed "host" string
            respond(member->getClientFd(), ":" + client->getNickname() + "!" + client->getUsername() + "@host" + " TOPIC #" + channelName + " :" + topic + "\r\n");
        }
    }
}