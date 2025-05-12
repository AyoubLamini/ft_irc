#include "../Includes/Bot.hpp"

int main(int argc, char **argv)
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <port> <hostname> <password>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    std::string hostname = argv[2];
    std::string password = argv[3];

    Bot bot(port, hostname, password);
    bot.connectToServer();
    bot.authenticate("weatherBot", "weather");
    bot.startBot();
    return 0;
}