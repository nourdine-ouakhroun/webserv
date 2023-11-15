

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror

SRCS =	main.cpp\
		Server.cpp\
		Request.cpp

RM = rm -f

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

all : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o  $(NAME)
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

