# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfabi <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/11 11:59:17 by jfabi             #+#    #+#              #
#    Updated: 2021/12/11 11:59:19 by jfabi            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ft_irc

SRCS		=	Client.cpp \
				Command.cpp \
				Message.cpp \
				RepliesCreator.cpp \
				Server.cpp \
				test2.cpp

OBJS		=	$(SRCS:.cpp=.o)

CPP			=	clang++

%.o:		%.cpp
			$(CPP) $(CFLAGS)   -c $< -o $(<:.cpp=.o)

CFLAGS		=	-std=c++98

RM			=	rm -f

all :		$(NAME)

$(NAME) :	$(OBJS)
			$(CPP) $(CFLAGS) -o $(NAME) $(OBJS) -I.

clean :
			$(RM) $(OBJS)

fclean 	:	clean
			$(RM) $(NAME)

re :		fclean all

.PHONY :	all clean fclean re
