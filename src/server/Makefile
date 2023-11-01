

XXC = c++
XXCFLAGS = -Wall -Wextra -Werror

SRC =	main.cpp\
		server.cpp\

OBJ = $(SRC:.cpp=.o)

NAME = SERVER

all : $(NAME)

$(NAME) : $(OBJ)
	$(XXC) $(XXCFLAGS) $(OBJ) -o  $(NAME)

%.o : %.cpp
	$(XXC) $(XXCFLAGS) -c $<

clean :
	rm -f $(OBJ)
fclean : clean
	rm -f $(NAME)
re : fclean $(NAME)
