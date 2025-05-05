#include "../Includes/Server.hpp"

void Server::inviteToChannel(Client *sender, const std::vector <std::string>& tokens)
{
    if (tokens.size() < 3)
        respond(sender->getClientFd(), ":ircserv 461 INVITE * :Not enough parameters\r\n");
    else
    {
        Client* target = getClientByNickname(tokens[1]);
        if (!target)
        {
            respond(sender->getClientFd(), ":ircserv 401 * " + tokens[1] + " :No such nick\r\n");
            return; 
        }
        Channel* channel = getChannel(storingName(tokens[2]));
        if (!channel)
        {
            respond(sender->getClientFd(), ":ircserv 403 * :" + tokens[2] + " :No such channel\r\n");
            return;
        }
        if (!channel->isUser(sender->getNickname())) // if the sender is in this channel
        {
            respond(sender->getClientFd(), ":ircserv 442 * :" + tokens[2] + " :You're not on that channel\r\n");
            return;
        }
        if (channel->isInviteOnly() && !channel->isOperator(sender->getNickname()))
        {
            respond(sender->getClientFd(), ":ircserv 482 * :" + tokens[2] + " :You're not channel operator\r\n");
            return;
        }
        if (channel->isUser(target->getNickname())) // if the target is alrdy in this channel
        {
            respond(sender->getClientFd(), ":ircserv 443 * :" + tokens[1] + " " + tokens[2] + " :is already on channel\r\n");
            return;
        }
        channel->addUser(target->getNickname());
        respond(target->getClientFd(), formatIrcMessage(sender->getNickname(), sender->getUsername(), "INVITE", target->getNickname(), channel->getName()));
        respond(sender->getClientFd(), ":ircserv 341 * :" + tokens[2] + " " + tokens[1] + "\r\n");
    }
}