#include "../Includes/Server.hpp"

void Server::channelMode(Client *client, const std::vector <std::string> &tokens)
{
    if (tokens.size() < 3)
    {
        respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + "MODE :Not enough parameters\r\n");
        return;
    }
    else 
    {
        Channel* channel = getChannel(storingName(tokens[1]));
        std::string modes = tokens[2];
        if (!channel)
        {
            respond(client->getClientFd(), ":ircserv 403 " + client->getNickname() + " " + tokens[1] + " :No such channel\r\n");
            return;
        }
        if (!channel->isUser(client->getNickname()))
        {
            respond(client->getClientFd(), ":ircserv 442 " + client->getNickname() + " " + tokens[1] + " :You're not on that channel\r\n");
            return;
        }
        if (!channel->isOperator(client->getNickname()))
        {
            respond(client->getClientFd(), ":ircserv 482 " + client->getNickname() + " " + tokens[1] + " :You're not channel operator\r\n");
            return;
        }
        if (!startsWith(modes, "+-") || modes.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " " + modes[0] + " : is unknown mode char to me\r\n");
            return;
        }
        int status = validateModes(client, channel, modes, tokens);
        if (status == 1)
            return;
    }
}


int Server::validateModes(Client *client, Channel *channel, std::string modes, const std::vector <std::string> &tokens)
{
    char sign = modes[0];
    size_t i = 1;
    size_t paramCounter = 3;
    while (i < modes.size())
    {
        if (isMode(modes[i]))
        {
            if (!requireParam(modes[i], sign) || paramCounter < 6)
            {
                int status = executeMode(client, channel, modes[i], sign, tokens, &paramCounter);
                if (status == 2)
                    return (status);    
            }
        }
        else if (isOtherSign(sign, modes[i]) && i > 1) // if char == '+', modes[i] should be '-' and vice vesra (but only when i > 1 == > means there is at least one mode char between signs)
        {
            sign == '+' ? sign = '-' : sign = '+';
            i++;
            if (i >= modes.size())
            {
                respond(client->getClientFd(), ":ircserv 461 " + client->getNickname()+ " " + sign + " :is unknown mode char to me\r\n");
                return (1);
            }
            while (i < modes.size())
            {
                if (!isMode(modes[i]))
                {
                    respond(client->getClientFd(), ":ircserv 461 " + client->getNickname()+ " " + modes[i] + " :is unknown mode char to me\r\n");
                    return (1);
                }
                if (!requireParam(modes[i], sign) || paramCounter < 6)
                {
                    int status = executeMode(client, channel, modes[i], sign, tokens, &paramCounter);
                    if (status == 2)
                        return (status);
                }
                i++;
            }
        }
        else
        {
            respond(client->getClientFd(), ":ircserv 461 " + client->getNickname()+ " " + modes[i] + " :is unknown mode char to me\r\n");
            return (1);
        }
            
        i++;
    }
    return (0);
}

int Server::executeMode(Client *client, Channel *channel, char mode, char sign, const std::vector <std::string> &tokens, size_t *counter)
{
    std::string param;
    std::string action;

    action.push_back(sign); // just for formatting message
    action.push_back(mode);
    if (requireParam(mode, sign))
    {
        if (*counter >= tokens.size())
        {
            respond(client->getClientFd(), ":ircserv 461 " + client->getNickname() + " MODE :Not enough parameters\r\n");
            return (2);
        }
        else
        {
            param = tokens[*counter];
            (*counter)++;
        }
    }
    std::cout  << "action: " << action << std::endl; 
    std::cout  << "param: " << param << std::endl; 


    if (mode == 'o')
    {
        if (!channel->isUser(param))
        {
            respond(client->getClientFd(), ":ircserv 401 " + client->getNickname() + " " + param +  " :No such nick/channel\r\n");
            return (2);
        }
        else if (sign == '+' && !channel->isOperator(param))
        {
            channel->addOperator(param);
        }
        else if (sign == '-' && channel->isOperator(param))
        {
            channel->deleteOperator(param);
        }
        sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
        return (0);
    }
    else if (mode == 'k')
    {
        if (sign == '+')
        {
            if (!isValidChannelKey(param))
            {
                respond(client->getClientFd(), ":ircserv 475 " + client->getNickname() + " " + param + " :Bad key\r\n");
                return (2);
            }
            else
            {
                channel->setKey(param);
                channel->setKeySet(true);
                sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
            }
        }
        else
        {
            channel->unsetKey();
            channel->setKeySet(false);
            sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
        }
    }
    else if (mode == 'l')
    {
        if (sign == '+')
        {
            int userLimit = validateUserLimit(param);
            if (userLimit == -1)
            {
                respond(client->getClientFd(), ":ircserv 696 " + client->getNickname() +  ":Invalid user limit (0-100)\r\n");
            }
            else
            {
                channel->setUserLimit(userLimit);
                channel->setUserLimitSet(true);
                sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
            }
        }
        else
        {
            channel->setUserLimitSet(false);
            sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
        }
    }
    else if (mode == 'i')
    {
        if (sign == '+')
        {
            channel->setInviteOnly(true);
        }
        else
        {
            channel->setInviteOnly(false); 
        }
        sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
    }
    else if (mode == 't')
    {
        if (sign == '+')
        {
            channel->setTopicLocked(true);
        }
        else
        {
            channel->setTopicLocked(false);
        }
        sendMessageToChannel(client, channel->getName(), action + " " + param, "MODE");
    }

    return (0);
}

bool isMode(char c)
{
    return (c == 'i' || c == 'k' || c == 't' || c == 'o' || c == 'l');
}
bool isOtherSign(char oldSign, char newSign)
{
    return ((oldSign == '+' && newSign == '-') || (oldSign == '-' && newSign == '+'));
}

bool requireParam(char mode, char sign)
{
    return ((mode == 'k' && sign == '+') || (mode == 'l' && sign == '+') || (mode == 'o'));
}