NAME = philo

SRCS = main.c init.c utils.c working.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address

LPTHREAD = -lpthread

$(NAME): $(OBJS)
	$(CC) $(LPTHREAD) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all