#include "../Includes/Server.hpp"

void signal_handler(int signum) 
{
    if (signum == SIGINT) {
        std::cout << "\nReceived SIGINT (Ctrl+C)" << std::endl;
    } else if (signum == SIGQUIT) {
        std::cout << "\nReceived SIGQUIT (Ctrl+\\)" << std::endl;
        // std::exit(0);  optionally exit on SIGQUIT
    }
}

void setUpSignals()
{
    std::signal(SIGINT, signal_handler);
    std::signal(SIGQUIT, signal_handler); 
}
