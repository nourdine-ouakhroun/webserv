NAME		=	webserv


SRCS		=	src/Parsing/Parser.cpp \
				src/Parsing/Checker.cpp \
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
				src/Server/main.cpp\
				src/Server/Server.cpp\
				src/Server/ParssingRequist.cpp\
				src/Server/Cgi.cpp\
				src/Utils/Poll.cpp\
				src/Server/ManageServers.cpp\
				src/Server/SocketDependencies.cpp\
				src/Exception/PollException.cpp\
				src/Server/ReadRequest.cpp

TEMPLATES =	src/Templates/*.tpp
HEADERS	=	src/Includes/*.hpp


CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion -O3 -I./src/Includes #-fsanitize=address

BIN			=	bin

OBJS		=	${SRCS:src/%.cpp=${BIN}/%.o}


all			:	${NAME}

${NAME}		: ${OBJS}
	${CPP} ${CPPFLAGS} $^ -o $@
	@echo "finish !!"

${BIN}/%.o	:	src/%.cpp ${HEADERS} ${TEMPLATES}
	@mkdir -p $(dir $@)
	${CPP} ${CPPFLAGS} -c $< -o $@

clean		:
	rm -rf ${BIN}

fclean		:	clean
	rm -rf ${NAME}

re		: fclean all

cclean 	:
	bash ~/Cleaner_42.sh

run		:	cclean	all
	./webserv configurations/default.conf
