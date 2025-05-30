/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:33:51 by ybouyzem          #+#    #+#             */
/*   Updated: 2025/05/30 15:33:57 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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