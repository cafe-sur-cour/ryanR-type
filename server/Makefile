SRC 	=		main.cpp \
			Server.cpp \
			ServerConfig.cpp

OBJ = $(SRC:.cpp=.o)

NAME = rtype_server

CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

LDFLAGS = -L./lib -lpacket -L./lib -lnetwork -L./lib -lbuffer

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./lib/packet
	make -C ./lib/network
	make -C ./lib/buffer
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS) $(CXXFLAGS)

clean:
	rm -f $(OBJ)
	make -C ./lib/packet clean
	make -C ./lib/network clean
	make -C ./lib/buffer clean

fclean: clean
	rm -f $(NAME)
	make -C ./lib/packet fclean
	make -C ./lib/network fclean
	make -C ./lib/buffer fclean

re: fclean all

.PHONY: all clean fclean re
