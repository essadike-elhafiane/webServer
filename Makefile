NAME = webserv
CC = c++ -Wall -Wextra -Werror -g -std=c++98
RM = rm -f

SRC = main.cpp server/server.cpp socketClient/socketClient.cpp request/request.cpp\
		response/response.cpp
includee = server/server.hpp socketClient/socketClient.hpp include/webserv.hpp request/request.hpp\
		response/response.hpp

obj = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(obj)
	$(CC) $(obj) -o $@

%.o: %.cpp $(includee)
	$(CC) -c $< -o $@

clean: 
	$(RM) $(obj)

fclean: clean
	$(RM) $(NAME)

re: fclean all
