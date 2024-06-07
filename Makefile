NAME = fdf

CFLAGS = -Wextra -Wall -Werror

HEADERS = -Iinclude -I MLX42/include

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

$(LIBMLX):
	@cd MLX42 && cmake -B build && cmake --build build -j4

%.o: %.c
	@echo "Compiling $< to $@..."
	$(CC) $(CFLAGS) -c $< -o $@ $(HEADERS)

$(NAME): $(LIBMLX) $(OBJS)
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "Linking object files to create $(NAME)..."
	@echo "\n\"$(NAME)\" file successfully created!"
	@echo "To run program: ./$(NAME) <map_file.fdf>"

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJS)

fclean: clean
	@echo "\nPerforming full clean..."
	rm -rf $(NAME)
	rm -rf ./MLX42/build

re: fclean all

.PHONY: all, clean, fclean, re
