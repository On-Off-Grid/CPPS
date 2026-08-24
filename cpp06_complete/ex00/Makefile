CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ex00

SRCS = main.cpp ScalarConverter.cpp

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
