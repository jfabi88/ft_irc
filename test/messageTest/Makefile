# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jfabi <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/05 14:37:24 by jfabi             #+#    #+#              #
#    Updated: 2022/01/05 14:37:26 by jfabi            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	message_test

SRCS		=	Message.cpp\
				utils.cpp \
                main.cpp

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
