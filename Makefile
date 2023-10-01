NAME = webserv
CC = c++ -Wall -Wextra -Werror -std=c++98
RM = rm -f

SRC = main.cpp

obj = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(obj)
	$(CC) $(obj) -o $@

%.o: %.cpp include/webserv.hpp
	$(CC) -c $< -o $@

clean: 
	$(RM) $(obj)

fclean: clean
	$(RM) $(NAME)

re: fclean all
