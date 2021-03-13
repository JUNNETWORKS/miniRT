INC=/usr/include
INCLIB=$(INC)/../lib
MLX_PATH = minilibx-linux
LIBFT_PATH = libft
MLX_MAKE = $(MAKE) -C $(MLX_PATH)
LIBFT_MAKE = $(MAKE) -C $(LIBFT_PATH)
LIBFT_LIB = ./libft/libft.a

NAME = miniRT
CC = gcc
HEADER_FILE = minirt.h
SRCS = main.c \
	   vec3.c \
	   drawers.c \
	   utils.c \
	   color_utils.c \
	   error_utils.c \
	   free_utils.c \
	   mlx_utils.c \
	   ptrarr_utils.c \
	   sphere.c \
	   plane.c \
	   intersection.c \
	   material.c \
	   debug.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

update:
	git submodule update --init --recursive

$(NAME): ${HEADER_FILE} ${OBJS}
	$(MLX_MAKE)
	$(LIBFT_MAKE)
	$(CC) -g -fsanitize=address -o $(NAME) $(SRCS) $(LIBFT_LIB) -L$(MLX_PATH) -lmlx -L$(INCLIB) -lXext -lX11 -lm -lbsd

clean:
	$(MLX_MAKE) clean
	$(LIBFT_MAKE) clean
	${RM} ${OBJS}

fclean: clean
	$(LIBFT_MAKE) fclean
	${RM} ${NAME}

re: fclean all

run: all
	./$(NAME)

.PHONY: all clean fclean re run update
