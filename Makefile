CC = cc
CFLAGS = -Wall -Werror -Wextra

SRCS = family.c \
		lidl_function.c \
		not_so_lidl.c \
		pipes.c \
		basic.c \
		main.c \
		path.c \
		basic2.c \
		debug_shit.c \
		builtins.c
	

NAME = minishell

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -lreadline 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(info Object files: $(OBJ))

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

# Mark these as not actual files
.PHONY: fclean clean all re