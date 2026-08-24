CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ex02

SRCS = main.cpp classes.cpp identify.cpp

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
