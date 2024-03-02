NAME	=	webserv


SRCS		=	src/Parsing/Checker.cpp \
				src/Parsing/Parser.cpp \
				src/Parsing/Data.cpp \
				src/Parsing/Patterns/LocationPattern.cpp \
				src/Parsing/Patterns/GeneralPattern.cpp \
				src/Parsing/Patterns/ServerPattern.cpp \
				src/Utils/String.cpp \
				src/Exception/ParsingException.cpp \
				src/Utils/ServerData.cpp \
				src/Exception/ServerException.cpp \
				src/Utils/Poll.cpp \
				src/Request/main.cpp \
				src/Server/Server.cpp \
				src/Server/Cgi.cpp \
				src/Exception/PollException.cpp \
				src/Server/ReadRequest.cpp \
				src/configurationTools.cpp \
				src/tools.cpp \
				src/Utils/Directives.cpp \
				src/Server/Servers.cpp \
				src/Server/Socket.cpp \
				src/Request/Request.cpp \
				src/Request/Response.cpp


HEADERS	=	src/Includes/*.hpp src/Request/*.hpp src/Parsing/*.hpp


CPP	=	c++

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion -O3 -I./src/Includes -fsanitize=address -g

BIN			=	bin

OBJS		=	${SRCS:src/%.cpp=${BIN}/%.o}


all			:	${NAME}

${NAME}		: ${OBJS}
	${CPP} ${CPPFLAGS} $^ -o $@
	@echo "finish !!"

${BIN}/%.o	:	src/%.cpp ${HEADERS} Makefile
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
