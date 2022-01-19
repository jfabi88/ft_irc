#Name
NAME =			irc

#Compiler
CC = 			g++
CFLAGS = 		-Wall -Wextra -Werror -std=c++98
LIBRARIES =
CFLAG = 		-c
OFLAG =			-o
IFLAG =			-I

#Make
MAKE = 			make -s -C
MAKE_CLEAN = 	make clean -s -C
MAKE_FCLEAN = 	make fclean -s -C

#Unzip
UNZIP =			unzip -X -o -q

#Shell
MKDIR =			mkdir -p
CP =			cp
RM =			rm -rf

#Directories
SOURCES_DIR =	./srcs
OBJECTS_DIR =	./objs
HEADERS_DIR =	./includes

#Files
FILES =			Channel.cpp \
				Command.cpp \
				Client.cpp  \
				Invite.cpp  \
				Kick.cpp    \
				Message.cpp \
				Quit.cpp    \
				RepliesCreator.cpp \
				Server.cpp  \
				test.cpp    \
				test2.cpp	\
				utils.cpp

#Srcs
SRCS =			$(foreach FILE, $(FILES), $(shell find $(SOURCES_DIR) -name $(FILE)))

#Objs
OBJS =			$(patsubst $(SOURCES_DIR)/%, $(OBJECTS_DIR)/%, $(SRCS:.cpp=.o))

$(OBJECTS_DIR)/%.o: $(SOURCES_DIR)/%.cpp
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(IFLAG) $(HEADERS_DIR) $(CFLAG) $(OFLAG) $@ $<

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBRARIES) $(OFLAG) $(NAME)
	@printf 'DONE!'

clean:
	@$(RM) $(OBJECTS_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all norminette clean fclean re