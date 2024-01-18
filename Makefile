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
				src/Utils/ResponseHeader.cpp \
				src/Request/main.cpp \
				src/Server/Server.cpp \
				src/Server/Cgi.cpp \
				src/Exception/PollException.cpp \
				src/Server/ReadRequest.cpp \
				src/configurationTools.cpp \
				src/tools.cpp \
				src/Utils/Directives.cpp \
				src/Server/ParssingRequest.cpp\
				src/Server/Servers.cpp \
				src/Server/Socket.cpp \
		src/Request/Request.cpp \
		src/Request/Response.cpp \
# 		src/Request/StatusCode.cpp
#		src/Request/Server.cpp
#		src/Utils/Select.cpp


TEMPLATES =	src/Templates/*.tpp
HEADERS	=	src/Includes/*.hpp src/Request/*.hpp


CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98  -O3 -I ./src/Includes -Wconversion #-fsanitize=address

BIN			=	bin

OBJS		=	${SRCS:src/%.cpp=${BIN}/%.o}


all			:	${NAME}

${NAME}		: ${OBJS}
	${CPP} ${CPPFLAGS} $^ -o $@
	@echo "finish !!"

${BIN}/%.o	:	src/%.cpp ${HEADERS} ${TEMPLATES} Makefile
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
