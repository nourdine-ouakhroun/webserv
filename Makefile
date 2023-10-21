NAME	=	websServ

SRCS	=	src/parser/main.cpp
HEADERS	=	includes/header.hpp

CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion

OBJS	=	${SRCS:.cpp=.o}

all	:	${NAME}

${NAME}	:	${OBJS}
	${CPP} ${CPPFLAGS} $< -o $@

%.o	:	%.cpp ${HEADERS}
	${CPP} ${CPPFLAGS} -c $<

clean	:
	rm -rf ${OBJS}

fclean	:	clean
	rm -rf ${NAME}

re	: fclean all
