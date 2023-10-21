NAME	=	websServ

SRCS	=	src/parser/main.cpp \
		src/parser/Parser.cpp \
		src/parser/Server.cpp \
		src/parser/Location.cpp
#		src/parser/InnerLocation.cpp \

HEADERS	=	src/parser/Parser.hpp \
		src/parser/Server.hpp \
		src/parser/Location.hpp	
#		src/parser/InnerLocation.hpp	\

CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion

OBJS	=	${SRCS:.cpp=.o}

all	:	${NAME}

${NAME}	:	${OBJS}
	${CPP} ${CPPFLAGS} $^ -o $@

%.o	:	%.cpp ${HEADERS}
	${CPP} ${CPPFLAGS} -c $< -o $@

clean	:
	rm -rf ${OBJS}

fclean	:	clean
	rm -rf ${NAME}

re	: fclean all
