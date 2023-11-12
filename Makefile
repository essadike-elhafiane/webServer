NAME = webserv
CC = c++ -Wall -Wextra -Werror -std=c++98 
RM = rm -f

SRC = main.cpp server/server.cpp Client/Client.cpp request/request.cpp\
		response/response.cpp config/parsing.cpp CGI/CGI.cpp CGI/mainCGI.cpp
includee = server/server.hpp Client/Client.hpp include/webserv.hpp request/request.hpp\
		response/response.hpp config/parsing.hpp CGI/CGI.hpp

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
