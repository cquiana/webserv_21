NAME	=	webserv

SRCS	=	ConfigParser.cpp \
			Http_config.cpp \
			Location_config.cpp \
			Server_config.cpp \
			Response.cpp \
			Request.cpp \
			utils.cpp \
			Client.cpp \
			Webserv.cpp \
			main.cpp \
			CGI.cpp \

HEADER	=	Webserv.hpp


OBJS = $(SRCS:.cpp=.o)

CL = clang++

#FLAGS = -std=c++98

FLAGS = -std=c++98 -Wall -Wextra -Werror

all: $(NAME)

%.o:%.cpp
	$(CL) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) $(HEADER)
	@$(CL) $(OBJS) $(FLAGS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all
	@echo "Project was remaked"

.PHONY: all re clean fclean
