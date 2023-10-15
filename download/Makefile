# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edraidry <edraidry@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/27 10:07:32 by edraidry          #+#    #+#              #
#    Updated: 2023/09/28 11:05:39 by edraidry         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RPN
HEADER = RPN.hpp
CC = c++
CFLAGS = -Wall -Wextra -Werror  -std=c++98
RM = rm -f
SRC = RPN.cpp \
main.cpp


obj =${SRC:.cpp=.o}
all:$(NAME)

$(NAME):$(obj)
	$(CC) $(CFLAGS) ${obj} -o $(NAME)


%.o: %.cpp $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	$(RM) $(obj)

fclean:clean
	$(RM) $(NAME)

re: fclean all