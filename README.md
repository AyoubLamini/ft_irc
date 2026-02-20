ft_irc

ft_irc is a custom implementation of an IRC (Internet Relay Chat) server, developed as part of the 1337 (42 Network) curriculum.

The goal of this project is to understand how real-world network servers work by building an IRC server from scratch using low-level socket programming in C++.

📌 Project Overview

This project consists of creating a fully functional IRC server that:

Handles multiple clients simultaneously

Uses TCP sockets for communication

Works in non-blocking mode

Follows the IRC protocol (RFC 1459 / 2812 core concepts)

Manages users, nicknames, channels, and operator privileges

The server can be tested using standard IRC clients such as:

irssi

weechat

HexChat

or even netcat

🛠 Technologies Used

C++ (C++98 standard)

Socket Programming (BSD sockets)

poll() or equivalent I/O multiplexing

Non-blocking file descriptors

Standard Template Library (STL)

⚙️ Features Implemented

✅ Multiple client connections
✅ Authentication with password
✅ Nickname management
✅ User registration
✅ Channel creation and management
✅ Private messages
✅ Channel operator commands
✅ Proper handling of IRC commands and replies

🧩 Supported Commands

Some of the core IRC commands implemented:

PASS

NICK

USER

JOIN

PART

PRIVMSG

KICK

INVITE

TOPIC

MODE

QUIT

🚀 How to Run
1️⃣ Compile
make
2️⃣ Run the server
./ircserv <port> <password>

Example:

./ircserv 6667 mypassword
3️⃣ Connect using an IRC client

Example with nc:

nc localhost 6667

Or configure an IRC client with:

Server: localhost

Port: 6667

Password: <your_password>

🧠 What I Learned

Deep understanding of TCP networking

Handling multiple clients using non-blocking I/O

Parsing and implementing a real communication protocol

Managing stateful server architecture

Writing clean and modular C++98 code

Debugging real-time networking systems

🎯 Project Goal

The objective of ft_irc is not only to build a chat server but to understand:

How real-world messaging systems work

How servers scale to handle multiple users

How protocols define communication rules

This project is one of the most advanced networking projects in the 1337 curriculum.  
