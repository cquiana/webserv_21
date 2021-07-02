NAME	=	webserv

SRCS	=	Webserv.cpp \
			Request.cpp \
			Client.cpp \
			Server.cpp \
			utils.cpp \
			main.cpp

HEADER	=	Webserv.hpp \
			Request.hpp \
			Client.hpp \
			Server.hpp \
			utils.hpp


OBJS = $(SRCS:.cpp=.o)

CL = clang++

FLAGS = -std=c++98 -g

#FLAGS = -std=c++98 -Wall -Wextra -Werror

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
