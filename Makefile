NAME = fdf

CC = clang-15

CFLAGS = -Wextra -Wall -Werror#-Wunreachable-code -Ofast

HEADERS = -I ./include -I $(LIBMLX)
LIBMLX = ./MLX42/build/libmlx42.a
LIBFT = ./lib/libft.a
LIBS = $(LIBMLX) $(LIBFT) -ldl -lglfw -pthread -lm

SRCS_DIR = ./src/

SRCS =	$(SRCS_DIR)bresenham.c \
		$(SRCS_DIR)draw_line.c \
		$(SRCS_DIR)fdf.c \
		$(SRCS_DIR)fdf_utils.c \
		$(SRCS_DIR)get_next_line.c \
		$(SRCS_DIR)render_control.c \
		$(SRCS_DIR)shift_color.c

OBJS = ${SRCS:.c=.o}

all: $(NAME)

# libmlx:
# 	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LIBS) $(HEADERS) -o $(NAME)

clean:
	@rm -rf $(OBJS)
# @rm -rf $(LIBMLX)/build

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY:
	all, clean, fclean, re
