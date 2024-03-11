NAME	=	webserv


SRCS	=	src/Parsing/Checker.cpp \
			src/Parsing/Parser.cpp \
			src/Parsing/Data.cpp \
			src/Parsing/Patterns/LocationPattern.cpp \
			src/Parsing/Patterns/GeneralPattern.cpp \
			src/Parsing/Patterns/ServerPattern.cpp \
			src/Exception/closeException.cpp \
			src/Exception/ParsingException.cpp \
			src/Exception/PollException.cpp \
			src/Exception/ServerException.cpp \
			src/Utils/ServerData.cpp \
			src/Utils/String.cpp \
			src/Utils/Poll.cpp \
			src/Server/Server.cpp \
			src/Server/ReadRequest.cpp \
			src/Server/Servers.cpp \
			src/Request/main.cpp \
			src/Server/Socket.cpp \
			src/Request/Request.cpp \
			src/Request/Response.cpp \
			src/configurationTools.cpp \
			src/tools.cpp


HEADERS	=	src/Includes/Cgi.hpp \
			src/Includes/ServerException.hpp \
			src/Includes/GeneralPattern.hpp \
			src/Includes/Checker.hpp \
			src/Includes/Data.hpp \
			src/Includes/ParsingException.hpp \
			src/Includes/LocationPattern.hpp \
			src/Includes/Logger.hpp \
			src/Includes/Parser.hpp \
			src/Includes/Poll.hpp \
			src/Includes/ServerPattern.hpp \
			src/Includes/ReadRequest.hpp \
			src/Includes/ServerData.hpp \
			src/Includes/Servers.hpp \
			src/Includes/Server.hpp \
			src/Includes/closeException.hpp \
			src/Includes/Socket.hpp \
			src/Includes/String.hpp


CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion -O3 -I./src/Includes -fsanitize=address -g

BIN			=	bin

OBJS		=	${SRCS:src/%.cpp=${BIN}/%.o}


all			:	${NAME}

${NAME}		: ${OBJS}  Makefile
	@mkdir -p html && echo "hello world" > html/index.html
	${CPP} ${CPPFLAGS} $^ -o $@
	@echo "finish !!"

${BIN}/%.o	:	src/%.cpp ${HEADERS}
	@mkdir -p $(dir $@)
	${CPP} ${CPPFLAGS} -c $< -o $@

clean		:
	rm -rf ${BIN}

fclean		:	clean
	rm -rf ${NAME}

re		: fclean all

cclean 	:
	clean

run		:	all
	./webserv configurations/default.conf

run_tmp		:	all
	./webserv configurations/tmp.conf
