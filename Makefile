CC = gcc

CFLAGS = -Wall -Wextra -Werror

NAME = server

SRC = srcs/main.c \
			srcs/server.c \
			srcs/error.c \
			srcs/routing.c \
			srcs/http_header.c \
			srcs/file.c \
			srcs/threads.c

OBJS = $(SRC:.c=.o)

HEADER = includes/

all: $(NAME)

$(NAME): $(OBJS) compile

$(OBJS): %.o: %.c 
	$(CC) -g $(CFLAGS) -I $(HEADER) -c $< -o $@

compile:
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

docker:
	sudo docker build -t web_server:1.0 .
	sudo docker compose up -d
	sudo docker ps

dclean:
	sudo docker compose down
	sudo docker image rm web_server:1.0
