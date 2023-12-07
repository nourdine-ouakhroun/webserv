NAME	=	webserv

SRCS	=	main.cpp \
			Request.cpp \
			Response.cpp \
			StatusCode.cpp \
			Server.cpp \

OBJS	=	$(SRCS:%.cpp=%.o)

CXX	=	c++

CXXFLAGS = -Wall -Wextra -Werror

all	:	$(NAME)

$(NAME)	: $(OBJS)
	make -C ../../
	$(CXX) $(CXXFLAGS) $(OBJS) ../../*.o -o $(NAME)
	@echo "finish !!"

%.o	: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean	:
	rm -rf $(OBJS)

fclean	:	clean
	rm -rf $(NAME)

re	: fclean all
