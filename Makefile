NAME	=	webServ

SRCS	=	src/main.cpp \
		src/Parser/Parser.cpp \
		src/Parser/Location.cpp \
		src/Models/GlobalModel.cpp \
		src/Models/ServerModel.cpp \
		src/Models/Data.cpp \
		src/Utils/String.cpp \
		src/Exception/ParsingException.cpp \
		src/Exception/ServerException.cpp \
		src/Server/Server.cpp \
		src/Utils/ServerData.cpp

HEADERS	=	src/Includes/*.hpp


CPP	=	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wconversion  -I./src/Includes# -fsanitize=address

BIN		=	bin

OBJS	=	${SRCS:src/%.cpp=${BIN}/%.o}

all	:	${NAME}

${NAME}	: ${OBJS}
	${CPP} ${CPPFLAGS} $^ -o $@

${BIN}/%.o	:	src/%.cpp ${HEADERS} ${BIN}
	@mkdir -p $(dir $@)
	@${CPP} ${CPPFLAGS} -c $< -o $@

${BIN} :
	mkdir -p $@

clean	:
	rm -rf ${BIN}

fclean	:	clean
	rm -rf ${NAME}

re	: fclean all
