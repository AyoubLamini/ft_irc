#include "../Includes/Server.hpp"

std::vector<std::string> splitedInput(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(input);

    while (std::getline(iss, token, delimiter))
    {
        size_t start = 0;
        while (start < token.size() && (token[start] == ' ' || token[start] == '\t')) {
            ++start;
        }
        token = token.substr(start);

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

std::vector<std::string> splitedJoin (const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(input);

    while (std::getline(iss, token, delimiter))
    {
        size_t start = 0;
        while (start < token.size() && (token[start] == ' ' || token[start] == '\t')) {
            ++start;
        }
        size_t end = token.size();
        while (end > start && (token[end - 1] == ' ' || token[end - 1] == '\t')) {
            --end;
        }
        token = token.substr(start, end - start);

        tokens.push_back(token); 
    }

    return tokens;
}



std::vector<std::string> topicSplit(const std::string& input)
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
                trailing += rest;
            }
            tokens.push_back(trailing);
            break;
        }
        tokens.push_back(token);
    }
    return tokens;
}

bool inCommandslist(std::string command)
{

    if (
        command == "NICK" 
        || command == "USER"
        || command ==  "PASS"
        || command == "JOIN"
        || command == "PRIVMSG"
        || command == "TOPIC"
        || command == "MODE"
        || command == "KICK"
        || command == "INVITE"
        || command == "QUIT"
    )
    {
        return true;
    }
    return false;
    
}


bool isValidNickname(const std::string& nickname)
{
    if (nickname.empty() || nickname.length() > 9)
        return false;

    if (!isalpha(nickname[0]) && (nickname[0] < '[' || nickname[0] > '`') && (nickname[0] < '{' || nickname[0] > '}'))
        return false;

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


bool isValidChannelName(const std::string& name)
{
    if (name.empty())
        return false;

    if (name[0] != '&' && name[0] != '#')
        return false;

    if (name.length() < 4 || name.length() > 50)
        return false;

    for (size_t i = 0; i < name.length(); ++i)
    {
        if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
            return false;
    }
    return true;
}

bool isValidChannelKey(const std::string& key)
{
    if (key.length() < 8 || key.length() > 24)
        return false;
    
    for (size_t i = 0; i < key.length(); ++i) {
        char c = key[i];
        if (c <= 32 || c >= 127)
            return false;
    }
    return true;
}
int validateUserLimit(const std::string& param)
{
    if (param.empty() || param.size() > 3) {
        return -1;
    }

    for (size_t i = 0; i < param.size(); ++i) {
        if (!std::isdigit(param[i])) {
            return -1;
        }
    }

    int value = atoi(param.c_str());
    if (value < 0 || value > 100) {
        return -1;
    }

    return value;
}

void printMessage(const std::vector<std::string>& tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << "token[" << i << "]=> ";
        std::cout << "\'" << tokens[i] << "\'" << std::endl;
    }
}


std::string formatIrcMessage(const std::string& prefixNick, const std::string& prefixUser, const std::string &hostname, const std::string& command, const std::string& target, const std::string& trailing)
{
    std::string message = ":" + prefixNick + "!" + prefixUser + "@" + hostname + " " + command + " " + target;
    if (!trailing.empty())
        message += " :" + trailing;
    message += "\r\n";
    return message;
}


std::string storingName(const std::string& str)
{
    std::string result = str;
    result = result.substr(1);
    for (size_t i = 0; i < result.length(); ++i)
        result[i] = std::toupper(result[i]);
    return result;
}

bool has_non_printables(const std::string& input)
{
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (!std::isprint(input[i]) && !std::isspace(input[i])) {
            return true;
        }
    }
    return false;
}
bool startsWith(const std::string& str, const std::string& set)
{
    return !str.empty() && set.find(str[0]) != std::string::npos;
}
bool has_newline(const std::string& s)
{
    return s.find('\n') != std::string::npos;
}

bool hasCR(const std::string& s)
{
    return s.find('\r') != std::string::npos;
}

size_t newLinePosition(const std::string& buffer) 
{
    size_t pos = buffer.find("\r\n");
    if (pos != std::string::npos)
        return pos;
    pos = buffer.find('\n');
    if (pos != std::string::npos)
        return pos;
    return std::string::npos;
}

bool newLineOnly(const std::string& buffer) 
{
    if (buffer.find("\r\n") == std::string::npos)
        return true;
    return false;
}

std::vector<std::string> mySplit(const std::string& str, char delimiter) 
{
    std::vector<std::string> result;
    std::string token;
    
    for (std::string::size_type i = 0; i < str.length(); ++i) {
        if (str[i] == delimiter) {
            result.push_back(token);
            token.clear();
        } else {
            token += str[i];
        }
    }
    result.push_back(token);
    return result;
}

void printBanner(int port) 
{
    std::cout << "\033[1;36m";
    std::cout << "          ___ ____   ____  \n";
    std::cout << "         |_ _|  _ \\ / ___|\n";
    std::cout << "          | || |_) | |  \n";
    std::cout << "          | ||  _ <| |___\n";
    std::cout << "         |___|_| \\_\\\\____|\n";
    std::cout << "\033[1;33m[INFO] IRCSERV is up and running!\n";
    std::cout << "\033[1;32m[INFO] Listening on port " << port << "... Waiting for clients.\n";
    std::cout << "\033[0m"; // Reset color
}
