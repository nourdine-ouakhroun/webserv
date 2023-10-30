NAME	=	webServ

SRCS	=	src/main.cpp \
		src/parser/Parser.cpp \
		src/parser/Server.cpp \
		src/parser/Location.cpp \
		src/Models/GlobalModel.cpp \
		src/Models/ServerModel.cpp \
		src/Models/Data.cpp \
		src/Utils/String.cpp \
		src/Exception/ParsingException.cpp
#		src/parser/InnerLocation.cpp \

HEADERS	=	src/parser/Parser.hpp \
		src/parser/Server.hpp \
		src/parser/Location.hpp \
		src/Models/Data.hpp \
		src/Utils/String.hpp \
		src/Exception/ParsingException.hpp \
		src/Models/GlobalModel.hpp \
		src/parser/InnerLocation.hpp

CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion# -fsanitize=address

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
