NAME = client server
BONUS_NAME = client_bonus server_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

COMMON_OBJ = utils.o
CLIENT_OBJ = client.o $(COMMON_OBJ)
SERVER_OBJ = server.o $(COMMON_OBJ)
CLIENT_BONUS_OBJ = client_bonus.o $(COMMON_OBJ)
SERVER_BONUS_OBJ = server_bonus.o $(COMMON_OBJ)

all: $(NAME)

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -o client

server: $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o server

bonus: $(BONUS_NAME)

client_bonus: $(CLIENT_BONUS_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_BONUS_OBJ) -o client_bonus

server_bonus: $(SERVER_BONUS_OBJ)
	$(CC) $(CFLAGS) $(SERVER_BONUS_OBJ) -o server_bonus

%.o: %.c minitalk.h
	$(CC) $(CFLAGS) -c $< -o $@

client_bonus.o server_bonus.o: minitalk_bonus.h

clean:
	$(RM) client.o server.o client_bonus.o server_bonus.o $(COMMON_OBJ)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re
