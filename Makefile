SRC_FILES = Src/main.cpp  Src/Server.cpp Src/Client.cpp  Src/Channel.cpp Src/Kick.cpp Src/Topic.cpp Src/Utils.cpp Src/Mode.cpp Src/Messages.cpp Src/Join.cpp Src/Invite.cpp Src/AuthRegisterClient.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
NAME = ircserv
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
RM = rm -f

all:	$(NAME)

$(NAME):$(OBJ_FILES)  Includes/Server.hpp Includes/Client.hpp 
		c++  $(CFLAGS)   $(OBJ_FILES) -o $(NAME)

%.o : %.cpp 
	c++ -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJ_FILES)


fclean:	clean
	$(RM) $(NAME)

re:	fclean $(NAME)