#include "../Includes/Server.hpp"

void Server::privateMessage(Client *client, std::string msg)
{
    std::vector <std::string> args = topicSplit(msg);
    if (args.size() < 3)
        respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " PRIVMSG :Not enough parameters\r\n");
    else
    {
        if (startsWith(args[1], "#&")) // target is a channel
        {
            std::string channelName = storingName(args[1]);
            sendMessageToChannel(client, channelName, args[2], "PRIVMSG");
        }
        else // target is a client
        {
            std::string clientName = args[1];
            sendMessageToClient(client, clientName, args[2]);
        }
    }
}

void Server::sendMessageToChannel(Client *sender, std::string channelName, std::string message, const std::string& messageType)
{
    Channel *channel = getChannel(channelName);
    if (channel == NULL)
    {
        respond(sender->getClientFd(), ":ircserv 403 " + sender->getNickname() + " " + channelName + " :No such channel\r\n");
        return;
    }
    if (!channel->isUser(sender->getNickname()))
    {
        respond(sender->getClientFd(), ":ircserv 404 " + sender->getNickname() + " " + channelName + " :Cannot send to channel\r\n");
        return;
    }

    std::vector <std::string> members = channel->getUsers();
    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        std::string reply;

        if (messageType == "PRIVMSG") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PRIVMSG", "#" + channelName, message);
        } else if (messageType == "JOIN") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "JOIN", "#" + channelName, "");
        } else if (messageType == "PART") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PART", "#" + channelName, "client disconnected");
        } else if (messageType == "MODE") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "MODE", "#" + channelName + " " + message, "");
        }
        if (member->getClientFd() != sender->getClientFd() || messageType == "JOIN")
            respond(member->getClientFd(), reply);
    }
}

void Server::sendMessageToClient(Client *sender, std::string targetName, std::string msg)
{
    Client *target = getClientByNickname(targetName);
    if (!target)
    {
        respond(sender->getClientFd(), ":ircserv 401 " + sender->getNickname() + " " + targetName + " :No such nick\r\n");
        return;
    }
    std::string reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PRIVMSG", targetName, msg);
    respond(target->getClientFd(), reply);
}