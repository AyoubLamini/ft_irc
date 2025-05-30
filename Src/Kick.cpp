#include "../Includes/Server.hpp"

void Server::kickMessage(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 3)
    {
        respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }

    std::string channelName = tokens[1];
    std::string targetNick = tokens[2];
    std::string reason = "No reason given";
    
    // Remove # if present in channel name
    if (channelName[0] == '#')
        channelName = storingName(channelName);

    if (!channelExist(channelName))
    {
        respond(client->getClientFd(), ":ircserv 403 " + client->getNickname() + " #" + channelName + " :No such channel\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);
    
    // Check if kicker is in the channel
    if (!channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 442 " + client->getNickname() + " #" + channelName + " :You're not on that channel\r\n");
        return;
    }
    
    // Check if kicker is an operator in the channel
    if (!channel->isOperator(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 482 " + client->getNickname() + " #" + channelName + " :You're not channel operator\r\n");
        return;
    }
    
    // Check if target user is in the channel
    if (!channel->isUser(targetNick))
    {
        respond(client->getClientFd(), ":ircserv 441 " + client->getNickname() + " " + targetNick + " #" + channelName + " :They aren't on that channel\r\n");
        return;
    }
    
    // Get the kick reason if provided (starting from token 3)
    if (tokens.size() > 3)
    {
        std::string fullReasonStr = "";
        
        // Reconstruct the full reason string from tokens[3] onward
        for (size_t i = 3; i < tokens.size(); ++i) {
            if (i > 3) fullReasonStr += " ";
            fullReasonStr += tokens[i];
        }
        
        // Use your topicSplit function to handle the reason with potential colon
        std::vector<std::string> reasonTokens = topicSplit(fullReasonStr);
        if (reasonTokens.size() > 0)
            reason = reasonTokens[0];
    }
    
    // Get the target client
    Client *targetClient = getClientByNickname(targetNick);
    if (!targetClient)
    {
        respond(client->getClientFd(), ":ircserv 401 " + client->getNickname() + " " + targetNick + " :No such nick/channel\r\n");
        return;
    }
    
    // Notify all users in the channel about the kick
    std::vector<std::string> members = channel->getUsers();
    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        if (member)
        {
            respond(member->getClientFd(), ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostName() + " KICK #" + channelName + " " + targetNick + " :" + reason + "\r\n");
        }
    }
    
    // Remove the user from the channel
    channel->deleteUser(targetNick);
    
    // If the channel becomes empty, check and clean up empty channels
    if (channel->getUserCount() == 0)
    {
        deleteEpmtyChannels();
    }
}