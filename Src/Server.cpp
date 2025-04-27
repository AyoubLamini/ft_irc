#include "../Includes/Server.hpp"

Server::Server()
{
    server_fd = -1;
}

Server::~Server()
{
    for (size_t i = 0; i < poll_fds.size(); ++i) 
    {
        close(poll_fds[i].fd);
    }
    if (server_fd != -1) 
    {
        close(server_fd);
    }
}

std::string Server::getPassword() const
{
    return (this->password);
}
int Server::getPort() const
{
    return (this->port);
}

void Server::setPassword(std::string password) 
{
    this->password = password;
}

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setServerFd(int server_fd)
{
    this->server_fd = server_fd;
}
int Server::getServerFd() const
{
    return (this->server_fd);
}

void Server::initializeServer()
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // creating TCP socket
    if (server_fd < 0) {
        perror("socket");
        return;
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Set socket options to reuse the address

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the struct

    server_addr.sin_family = AF_INET; // Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available IP addresses
    server_addr.sin_port = htons(port); // Convert the port to network byte order

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        close(server_fd);
        return;
    }

    struct pollfd pfd;
    pfd.fd = server_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    poll_fds.push_back(pfd);
}


void Server::run()
{
    while (true) 
    {
        int poll_count = poll(poll_fds.data(), poll_fds.size(), -1);
        if (poll_count < 0) 
        {
            perror("poll");
            break;
        }
        for (size_t i = 0; i < poll_fds.size(); ++i)
        {
            if (poll_fds[i].revents & POLLIN) // if true => there is data to read
            {
                if (poll_fds[i].fd == server_fd)  // if true, means its server socket, means its a new connection
                {
                    acceptClient();
                } 
                else 
                {
                    readClient(poll_fds[i].fd);
                }
            }
            if (poll_fds[i].revents & POLLOUT) 
            {
                writeClient(poll_fds[i].fd); 
            }
        }
        ClearDisconnectedClients();
    }
}

void Server::ClearDisconnectedClients() 
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getStatus() == false && _Clients[i]->getBuffer().empty()) 
        {
            deleteUserFromChannels(_Clients[i]);
            deleteEpmtyChannels();
            deleteClientData(_Clients[i]);
            _Clients.erase(_Clients.begin() + i);
            --i;
        }
    }
}

void Server::deleteEpmtyChannels()
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        if (_Channels[i]->getUserCount() == 0) 
        {
            delete _Channels[i];
            _Channels.erase(_Channels.begin() + i);
            --i;
        }
    }
}


void Server::respond(int client_fd, const std::string& message)
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL)
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    client->appendMessage(message);
    for (size_t i = 0; i < poll_fds.size(); ++i) 
    {
        if (poll_fds[i].fd == client_fd) 
        {
            poll_fds[i].events |= POLLOUT;
            break;
        }
    }
}

void Server::writeClient(int client_fd)
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL)
    {
        std::cout << "Client not found" << std::endl;
        return;
    }
    std::string message = client->getBuffer();
    size_t bytes_sent = send(client_fd, message.c_str(), message.length(), 0);
    if (bytes_sent < 0) 
    {
        perror("send");
        close(client_fd);
        return;
    }
    client->eraseMessage(bytes_sent);
    if (client->getBuffer().empty())
    {
        for (size_t i = 0; i < poll_fds.size(); ++i) 
        {
            if (poll_fds[i].fd == client_fd) {
                poll_fds[i].events &= ~POLLOUT;
            break;
        }
    }
}
}


void Server::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        return;
    }
    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags == -1 || fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl");
        close(client_fd);
        return;
    }

    std::cout << "Client connected" << std::endl;

    // Add the new client to the poll_fds vector
    struct pollfd pfd;
    pfd.fd = client_fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    poll_fds.push_back(pfd);
    // create a new Client object and add it to the _Clients vector
    Client *new_client = new Client(client_fd, client_addr);
    _Clients.push_back(new_client);
}






void Server::readClient(int client_fd) // Read client socket
{
    Client *client = getClientByFd(client_fd);
    if (client == NULL) 
    {
        std::cout << "Client not found" << std::endl;
        return;
    }

    std::string msg = recvMessage(client_fd);
    std::vector<std::string> tokens = splitedInput(msg, ' ');
    if (!tokens.empty())
    {
        // Authentication and Registration
        if (!client->isAuthenticated() || !client->isRegistered()) 
        {
            if (!client->isAuthenticated())
                authenticateClient(client, tokens);
            else
            {
                registerClient(client, tokens);
                checkRegistration(client);
            }
        }
        // Commands
        else
        {
            if (tokens[0] == "PASS" || tokens[0] == "NICK" || tokens[0] == "USER")
            {
                respond(client->getClientFd(), ":ircserv 462 * :Already registered\r\n");
                return; 
            }
            else if (inCommandslist(tokens[0]))
            {
                processCommands(client, tokens);
            }
            else
            {
                respond(client->getClientFd(), ":ircserv 421 * :Unknown command\r\n");
                return; 
            }
        }
    }
    else
        std::cout << "Tokens empty" << std::endl;
}



std::string Server::recvMessage(int client_fd) // Recive Message
{
    char buffer[1024];
    Client *client = getClientByFd(client_fd);
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) 
    {
        if (bytes_read == 0)
        {
            std::cout << "Client disconnected" << std::endl;
            client->setStatus(false);
        }
        else 
        {
            perror("recv");
        }
        return std::string(); // Return empty string on error
    }
    buffer[bytes_read] = '\0';
    std::string msg =  std::string(buffer);
    if (msg.back() == '\n')
        msg.pop_back();
    if (msg.back() == '\r')
        msg.pop_back();
    return msg;
}

    
void Server::authenticateClient(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens[0] == "PASS")
    {
        if (tokens.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 461 * :Not enough parameters\r\n");
            client->setStatus(false);
            return;
        }
        else if (tokens[1] == this->password)
        {
            client->setAuthenticated(true);
            std::cout << "Client authenticated" << std::endl;
            return;
        }
        else
        {
            respond(client->getClientFd(), ":ircserv 464 * :Password incorrect\r\n");
            client->setStatus(false);
            return;
        }
    }
    else if (inCommandslist(tokens[0]))
    {
        respond(client->getClientFd(), ":ircserv 451 * :You have not registered\r\n");
        client->setStatus(false);
        return;
    }
    else
    {
        respond(client->getClientFd(), ":ircserv 421 * :Unknown command\r\n");
        client->setStatus(false);
        return;
    }
}






void Server::registerClient(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens[0] == "NICK" && client->getNickname().empty()) // Nickname Validation
    {
        if (tokens.size() < 2)
        {
            respond(client->getClientFd(), ":ircserv 431 * :No nickname given\r\n");
            return;
        }
        else if (nickExists(tokens[1]))
        {
            respond(client->getClientFd(), ":ircserv 433 * " + tokens[1] + " :Nickname is already in use\r\n");
            return;
        }
        else if(!isValidNickname(tokens[1]))
        {
            respond(client->getClientFd(), ":ircserv 432 * " + tokens[1] + " :Erroneous nickname\r\n");
            return;
        }
        else
        {
            client->setNickname(tokens[1]);
            std::cout << "Nickname entered" << std::endl;
            return;
        }
      
    }
    else if (tokens[0] == "USER" && client->getUsername().empty()) // USERNAME VALIDATION
    {
        if (tokens.size() < 5)
        {
            respond(client->getClientFd(), ":ircserv 461 USER * :Not enough parameters\r\n");
            return;
        }
        else if (tokens.size() > 15)
        {
            respond(client->getClientFd(), ":ircserv 461 USER * :Too many parameters\r\n");
            return;
        }
        else
        {
            client->setUsername(tokens[1]);
            std::cout << "username entered" << std::endl;
            return;
        }

    }
    else if (inCommandslist(tokens[0]))
    {
        respond(client->getClientFd(), ":ircserv 451 * :You have not registered\r\n");
        return;
    }
    else
    {
        respond(client->getClientFd(), ":ircserv 421 * :Unknown command\r\n");
        return;
    }
}

void Server::checkRegistration(Client *client)
{
    if (client->getUsername() != "" && client->getNickname() != "")
    {
        client->setRegistered(true);
        respond(client->getClientFd(), ":ircserv 001 " + client->getNickname() + " :Welcome to the IRC server\r\n");
        std::cout << "Client registered" << std::endl;
    }
}


void Server::processCommands(Client *client, const std::vector <std::string>& tokens)
{
    if (tokens[0] == "JOIN")
    {
        joinMessage(client, tokens);
    }
    else if (tokens[0] == "TOPIC")
    {
        topicMessage(client, tokens);
    }
    else if (tokens[0] == "MODE")
    {
        modeMessage(client, tokens);
    }
}

std::vector<std::string> Server::topicSplit(const std::string& input)
{
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token)
    {
        if (!token.empty() && token[0] == ':') 
        {
            std::string trailing = token.substr(1);
            std::string rest;
            std::getline(iss, rest);
            if (!rest.empty()) 
            {
                trailing += " " + rest;
            }
            tokens.push_back(trailing);
            break;
        }
        tokens.push_back(token);
    }
    return tokens;
}

void Server::modeMessage(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 3)
    {
        respond(client->getClientFd(), ":ircserv 461 MODE :Not enough parameters\r\n");
        return;
    }

    std::string channelName = tokens[1];
    // Remove # if present
    if (channelName[0] == '#')
        channelName = storingName(channelName);

    if (!channelExist(channelName))
    {
        respond(client->getClientFd(), ":ircserv 403 " + client->getNickname() + " " + channelName + " :No such channel\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);
    
    // Check if user is in the channel
    if (!channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 442 " + client->getNickname() + " " + channelName + " :You're not on that channel\r\n");
        return;
    }

    // Check if user is an operator
    if (!channel->isOperator(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 482 " + client->getNickname() + " " + channelName + " :You're not channel operator\r\n");
        return;
    }

    std::string modeString = tokens[2];
    if (modeString.empty())
    {
        respond(client->getClientFd(), ":ircserv 461 MODE :Not enough parameters\r\n");
        return;
    }

    bool addMode = true;
    if (modeString[0] == '+')
        addMode = true;
    else if (modeString[0] == '-')
        addMode = false;
    else
    {
        respond(client->getClientFd(), ":ircserv 472 " + client->getNickname() + " " + modeString + " :Unknown mode\r\n");
        return;
    }

    // Process mode flags
    for (size_t i = 1; i < modeString.length(); ++i)
    {
        char mode = modeString[i];
        switch (mode)
        {
            case 't': // Topic restriction mode
            {
                channel->setTopicLocked(addMode);
                // Notify all users in the channel about the mode change
                std::vector<std::string> members = channel->getUsers();
                for (size_t j = 0; j < members.size(); ++j)
                {
                    Client *member = getClientByNickname(members[j]);
                    if (member)
                    {
                        std::string modeChange;
                        if (addMode)
                            modeChange = "+t";
                        else
                            modeChange = "-t";

                        respond(member->getClientFd(), ":" + client->getNickname() + "!" + client->getUsername() + "@host MODE #" + channelName + " " + modeChange + "\r\n");
                    }
                }
                break;
            }
            // You can implement other modes here as needed
            // case 'i', 'k', 'o', 'l' etc.
            
            default:
                respond(client->getClientFd(), ":ircserv 472 " + client->getNickname() + " " + mode + " :Unknown mode\r\n");
                break;
        }
    }
}


void Server::topicMessage(Client *client, const std::vector<std::string>& tokens)
{
    if (tokens.size() < 2)
    {
        respond(client->getClientFd(), ":ircserv 461 TOPIC * :Not enough parameters\r\n");
        return;
    }

    std::string channelName = tokens[1];
    // Remove # if present
    if (channelName[0] == '#')
        channelName = storingName(channelName);

    if (!channelExist(channelName))
    {
        respond(client->getClientFd(), ":ircserv 403 * :" + channelName + " :No such channel\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);
    
    // Check if user is in the channel
    if (!channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 442 * :" + channelName + " :You're not on that channel\r\n");
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

    // Get the topic from the tokens
    std::string topic;
    std::vector<std::string> topicTokens = topicSplit(tokens[2]);
    if (topicTokens.size() > 0)
        topic = topicTokens[0];
    else
        topic = "";

    // Set the topic
    channel->setTopic(topic);
    
    // Notify all users in the channel of the topic change
    std::vector<std::string> members = channel->getUsers();
    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        if (member)
        {
            respond(member->getClientFd(), ":" + client->getNickname() + "!" + client->getUsername() + "@host TOPIC #" + channelName + " :" + topic + "\r\n");
        }
    }
}

void Server::joinMessage(Client *client, const std::vector <std::string>& tokens)
{
    std::vector <std::string> names;
    std::vector <std::string> keys;

    if (tokens.size() < 2)
    {
        respond(client->getClientFd(), ":ircserv 461 JOIN * :Not enough parameters\r\n");
        return;
    }

    names = splitedInput(tokens[1], ',');
    if (tokens.size() > 2)
        keys = splitedInput(tokens[2], ',');
    if (names.size() + keys.size() > 15)
    {
        respond(client->getClientFd(), ":ircserv 470 * :Too many JOIN parameters\r\n");
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
            respond(client->getClientFd(), ":ircserv 403 * :" + names[i] + " :Bad channel name\r\n");
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
                createChannel(client, channelName, keys[i]);
                std::cout << "Client created channel" << std::endl;
            }
        }
    }
}

// Channel managment 

bool Server::channelExist(std::string channel)
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        if (_Channels[i]->getName() == channel) 
        {
            return true;
        }
    }
    return false;
}

Channel *Server::getChannel(std::string channel)
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        if (_Channels[i]->getName() == channel) 
        {
            return _Channels[i];
        }
    }
    return NULL;
}

void Server::createChannel(Client *client, std::string name, std::string key)
{
    Channel *channel = new Channel(name, key);
    std::cout << "Channel created:|" << channel->getName() << "|"  << std::endl;
    channel->addOperator(client->getNickname());
    channel->addUser(client->getNickname());
    _Channels.push_back(channel);
    respond(client->getClientFd(), formatIrcMessage(client->getNickname(), client->getUsername(), "JOIN", " #" + channel->getName(), ""));
}

void Server::joinChannel(Client *client, std::string name, std::string key)
{
    Channel *channel = getChannel(name);
    if (channel->isKeySet() && channel->getKey() != key)
    {
        respond(client->getClientFd(), ":ircserv 475 * : #" + name + " :Cannot join channel (+k) - bad key\r\n");
    }
    else if (channel->isUser(client->getNickname()))
    {
        respond(client->getClientFd(), ":ircserv 443 * : #" + name + " :You are already a member of the channel\r\n");
    }
    else if (channel->isInviteOnly())
    {
        respond(client->getClientFd(), ":ircserv 473 * : #" + name + " :Cannot join channel (+i) - invite only\r\n");
    }
    else if (channel->isUserLimitSet() && channel->getUserLimit() <= channel->getUserCount())
    {
        respond(client->getClientFd(), ":ircserv 471 * : #" + name + " :Cannot join channel (+l) - user limit reached\r\n");
    }
    else
    {
        channel->addUser(client->getNickname());
        sendMessageToChannel(client, channel->getName(), "", "JOIN");
    }

}




// -----------------------------------------------------------------------------------------------
void Server::sendMessageToChannel(Client *sender, std::string channelName, std::string message, const std::string& messageType)
{
    // print channels
    std::cout << "Channels: " << std::endl;
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        std::cout << "Channel[" << i << "] => " << _Channels[i]->getName() << std::endl;
    }
    Channel *channel = getChannel(channelName);
    if (channel == NULL)
    {
        std::cout << "NO SUCH CHANNEL: " << channelName << std::endl;
        respond(sender->getClientFd(), ":ircserv 403 * :" + channelName + " :No such channel\r\n");
        return;
    }
    std::vector <std::string> members = channel->getUsers();
    std::cout << "Members size: " << members.size() << std::endl;

    for (size_t i = 0; i < members.size(); ++i)
    {
        Client *member = getClientByNickname(members[i]);
        std::string reply;
        // exit(0);
        // Check message type and format accordingly
        if (messageType == "PRIVMSG") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PRIVMSG", " #" + channelName, message);
        } else if (messageType == "JOIN") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "JOIN", " #" + channelName, "");
        } else if (messageType == "PART") {
            reply = formatIrcMessage(sender->getNickname(), sender->getUsername(), "PART", " #" + channelName, "client disconnected");
        }
        respond(member->getClientFd(), reply);
    }
}

bool Server::nickExists(std::string nickname)
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getNickname() == nickname) 
        {
            return true;
        }
    }
    return false;
}

Client *Server::getClientByFd(int client_fd)
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getClientFd() == client_fd) 
        {
            return _Clients[i];
        }
    }
    return NULL;
}

Client *Server::getClientByNickname(const std::string& nickname)
{
    for (size_t i = 0; i < _Clients.size(); ++i) 
    {
        if (_Clients[i]->getNickname() == nickname) 
        {
            return _Clients[i];
        }
    }
    return NULL;
}

void Server::deleteUserFromChannels(Client *client)
{
    for (size_t i = 0; i < _Channels.size(); ++i) 
    {
        Channel *channel = _Channels[i];
        if (channel->isUser(client->getNickname()))
        {
            sendMessageToChannel(client, channel->getName(), "", "PART");
            channel->deleteUser(client->getNickname());
        }
    }
}

void Server::deleteClientData(Client *client)
{
    for (size_t i = 0; i < this->poll_fds.size(); i++)
    {
        if (poll_fds[i].fd == client->getClientFd())
        {
            poll_fds.erase(poll_fds.begin() + i);
            break;
        }
    }
    delete client;
}



// Helper Functions ------------------------------------------------------------------------------------------------


std::vector<std::string> Server::splitedInput(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(input);

    while (std::getline(iss, token, delimiter))
    {
        // Remove leading spaces
        size_t start = 0;
        while (start < token.size() && (token[start] == ' ' || token[start] == '\t')) {
            ++start;
        }
        token = token.substr(start);

        // Remove trailing spaces
        size_t end = token.size();
        while (end > start && (token[end - 1] == ' ' || token[end - 1] == '\t')) {
            --end;
        }
        token = token.substr(0, end);

        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

bool Server::inCommandslist(std::string command)
{

    if (
        command == "NICK" 
        || command == "USER"
        || command ==  "PASS"
        || command == "PONG"
        || command == "PING"
        || command == "JOIN"
        || command == "PART"
        || command == "PRIVMSG"
        || command == "NOTICE"
        || command == "TOPIC"
        || command == "MODE"
        || command == "KICK"
        || command == "INVITE"
        || command == "WHOIS"
    )
    {
        return true;
    }
    return false;
    
}


bool Server::isValidNickname(const std::string& nickname)
{
    if (nickname.empty() || nickname.length() > 9)
        return false;

    // First character must be a letter or special
    if (!isalpha(nickname[0]) && (nickname[0] < '[' || nickname[0] > '`') && (nickname[0] < '{' || nickname[0] > '}'))
        return false;

    // Remaining characters: letter, digit, special, or '-'
    for (size_t i = 1; i < nickname.length(); ++i)
    {
        if (!isalnum(nickname[i]) && 
            (nickname[i] < '[' || nickname[i] > '`') && 
            (nickname[i] < '{' || nickname[i] > '}') && 
            nickname[i] != '-')
            return false;
    }
    return true;
}


bool Server::isValidChannelName(const std::string& name)
{
    if (name.empty())
        return false;

    // Must start with &, #, +, or !
    if (name[0] != '&' && name[0] != '#' && name[0] != '+' && name[0] != '!')
        return false;

    // Length between 4 and 50 characters
    if (name.length() < 4 || name.length() > 50)
        return false;

    // Cannot contain space, comma, or ASCII 7 (BEL)
    for (size_t i = 0; i < name.length(); ++i)
    {
        if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
            return false;
    }
    return true;
}

void Server::printMessage(const std::vector<std::string>& tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "token[" << i << "]=> ";
        std::cout << "\'" << tokens[i] << "\'" << std::endl;
    }
}


std::string Server::formatIrcMessage(const std::string& prefixNick, const std::string& prefixUser, const std::string& command, const std::string& target, const std::string& trailing)
{
    std::string message = ":" + prefixNick + "!" + prefixUser + "@host " + command + " " + target;
    if (!trailing.empty())
        message += " :" + trailing;
    message += "\r\n";
    return message;
}


std::string Server::storingName(const std::string& str)
{
    std::string result = str;
    result = result.substr(1);
    for (size_t i = 0; i < result.length(); ++i)
        result[i] = std::toupper(result[i]);
    return result;
}
