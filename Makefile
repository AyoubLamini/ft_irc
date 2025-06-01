SRC_FILES = Src/main.cpp  Src/Server.cpp Src/Client.cpp  Src/Channel.cpp Src/Kick.cpp Src/Topic.cpp Src/Utils.cpp Src/Mode.cpp Src/Messages.cpp Src/Join.cpp Src/Invite.cpp Src/AuthRegisterClient.cpp 
OBJ_FILES = $(SRC_FILES:.cpp=.o)
NAME = ircserv
CFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
RM = rm -f

SRC_BONUS = Bonus/main.cpp Bonus/Bot.cpp Src/Utils.cpp 

OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

NAME_BONUS = weatherBot

all: $(NAME)

$(NAME): $(OBJ_FILES)  Includes/Server.hpp Includes/Client.hpp Includes/Channel.hpp
		c++  $(CFLAGS)   $(OBJ_FILES) -o $(NAME)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_BONUS) Includes/Server.hpp Includes/Client.hpp Includes/Channel.hpp Includes/Bot.hpp Includes/WeatherData.hpp
		c++  $(CFLAGS)   $(OBJ_BONUS) -o $(NAME_BONUS)

bonus/%.o : Bonus/%.cpp
	c++ -c $(CFLAGS) $< -o $@


%.o : %.cpp
	c++ -c $(CFLAGS) $< -o $@


clean:
	$(RM) $(OBJ_FILES) $(OBJ_BONUS)


fclean:	clean
	$(RM) $(NAME) $(NAME_BONUS)

re:	fclean all

