NAME		=	webserv


SRCS		=	src/main.cpp \
				src/tools.cpp \
				src/configurationTools.cpp \
				src/Parsing/Parser.cpp \
				src/Parsing/Checker.cpp \
				src/Parsing/Data.cpp \
				src/Parsing/Patterns/LocationPattern.cpp \
				src/Parsing/Patterns/GeneralPattern.cpp \
				src/Parsing/Patterns/ServerPattern.cpp \
				src/Utils/String.cpp \
				src/Exception/ParsingException.cpp \
				src/Utils/ServerData.cpp \
				src/Exception/ServerException.cpp \
				src/Server/Server.cpp \
				src/Utils/Poll.cpp \
				src/Utils/ResponseHeader.cpp \
#				src/Utils/Select.cpp

TEMPLATES	=	src/Templates/*.tpp
HEADERS		=	src/Includes/*.hpp


CPP			=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion -O3 -I./src/Includes# -fsanitize=address

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

run		:	all
	./webserv configurations/default.conf