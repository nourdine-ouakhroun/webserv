

XXC = c++
XXCFLAGS = -Wall -Wextra -Werror

SRC =	main.cpp\
		server.cpp\

OBJ = $(SRC:.cpp=.o)

NAME = SERVER

all : $(OBJ)
	$(XXC) $(XXCFLAGS) $(OBJ) -o  $(NAME)
%.o : %.cpp
	$(XXC) $(XXCFLAGS) -c $<
