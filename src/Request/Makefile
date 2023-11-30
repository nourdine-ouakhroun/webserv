NAME	=	webserv

SRCS	=	main.cpp \
			Request.cpp \
			RequestLine.cpp \
			RequestHeader.cpp \
			RequestBody.cpp \
			StatusCode.cpp \
			Response.cpp \
			Server.cpp \

OBJS	=	$(SRCS:%.cpp=%.o)

CXX	=	c++

CXXFLAGS = -Wall -Wextra -Werror

all	:	$(NAME)

$(NAME)	: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "finish !!"

%.o	: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean	:
	rm -rf $(OBJS)

fclean	:	clean
	rm -rf $(NAME)

re	: fclean all
