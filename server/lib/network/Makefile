SRC = 	LinuxNetwork.cpp  		\
	  	WindowsNetwork.cpp      	\


OBJ = $(SRC:.cpp=.o)

NAME = libnetwork.a

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	mv $(NAME) ../$(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f ../$(NAME)

re: fclean all

.PHONY: all clean fclean re
