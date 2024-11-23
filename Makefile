NAME = ircserv

BOT = Bot

HEADER = headers/client.hpp headers/Parser.hpp  headers/authenticate.hpp\
		headers/server.hpp headers/polldt.hpp headers/IRCReplies headers/utils.hpp \
		headers/Channel.hpp headers/ircExceptions.hpp

SRC =  src/client.cpp src/Parser.cpp src/main.cpp src/server.cpp  \
		src/polldt.cpp src/authenticate.cpp src/utils.cpp src/Commands.cpp src/Channel.cpp \
		src/ChannelCommands.cpp 

HBOT = headers/Bot.hpp

BSRC = src/Bot.cpp


OBJ = $(SRC:.cpp=.o)

BOBJ = $(BSRC:.cpp=.o)

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98 -g

%.o : %.cpp
	c++ $(FLAGS) -c $< -o $@

$(NAME): $(OBJ) $(BOBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) 
	$(CC) $(FLAGS) $(BOBJ) -o $(BOT) 

all : $(NAME)

clean:
	rm -rf $(OBJ)
	rm -rf $(BOBJ)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(BOT)

re: fclean all

.PHONY: all clean fclean re
