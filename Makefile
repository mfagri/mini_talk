# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mfagri <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/29 11:29:34 by mfagri            #+#    #+#              #
#    Updated: 2022/01/05 19:09:46 by mfagri           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME1 = server

NAME2 = client

CC = gcc

INC = minitalk.h

CFLAGS = -Wall -Wextra -Werror

SOURCES =	minitalk_utils.c\

OBJECTS = $(SOURCES:.c=.o) 

all:$(NAME1) $(NAME2)

$(NAME1) : server.c $(INC)
	 		$(CC) $(CFLAGS) server.c $(SOURCES) -o server
$(NAME2) : client.c $(INC)
			$(CC) $(CFLAGS) client.c $(SOURCES) -o client
clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME1) $(NAME2)

re: fclean all
