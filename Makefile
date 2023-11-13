NAME	=	webserv


SRCS	= src/Parser/Parser.cpp \
		src/Parser/Location.cpp \
		src/Models/GlobalModel.cpp \
		src/Models/ServerModel.cpp \
		src/Models/Data.cpp \
		src/Utils/String.cpp \
		src/Exception/ParsingException.cpp \
		src/Exception/ServerException.cpp \
		src/Utils/ServerData.cpp\
		src/Server/main.cpp\
		src/Server/Server.cpp\
		src/Server/ParssingRequist.cpp\
		src/Server/Cgi.cpp\



HEADERS	=	src/Includes/*.hpp


CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion -O3 -I./src/Includes# -fsanitize=address

BIN		=	bin

OBJS	=	${SRCS:src/%.cpp=${BIN}/%.o}


all	:	${NAME}

${NAME}	: ${OBJS}
	${CPP} ${CPPFLAGS} $^ -o $@
	@echo "finish !!"

${BIN}/%.o	:	src/%.cpp ${HEADERS}
	@mkdir -p $(dir $@)
	${CPP} ${CPPFLAGS} -c $< -o $@

clean	:
	rm -rf ${BIN}

fclean	:	clean
	rm -rf ${NAME}

re	: fclean all
