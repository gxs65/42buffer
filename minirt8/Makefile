END=$'\x1b[0m
GREEN=$'\x1b[32m

SRCS 			= $(wildcard srcs/*/*.c) $(wildcard srcs/*.c)
OBJS 			= $(SRCS:.c=.o)

CC				= clang
CP				= cp
RM				= rm -fr
NAME			= miniRT

ifeq ($(shell uname), Linux)
MLX_DIR			= mlx-linux/
MLX_NAME		= libmlx.a
MLX				= $(MLX_DIR)/$(MLX_NAME)
MLX_FLAGS		= -L ./mlx-linux/ -lmlx -Ilmlx -lXext -lX11 -lm
else
MLX_DIR			= mlx-macos/
MLX_NAME		= libmlx.dylib
MLX				= $(MLX_DIR)/$(MLX_NAME)
MLX_FLAGS		= -framework OpenGL -framework Appkit -lz
endif

INCLUDES		+= -I libft/includes/
INCLUDES		+= -I includes/
INCLUDES		+= -I $(MLX_DIR)

CFLAGS 			= -Wall -Wextra -Werror $(INCLUDES)

LIBFT			= ./libft/libft.a
LIBFT_DIR		= ./libft

all : $(NAME)

$(NAME) : $(OBJS) $(LIBFT) $(MLX) 
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX) $(MLX_FLAGS) -o $(NAME)
		@if [ $(shell uname) = Darwin ]; then\
			$(CP) $(MLX) .;\
		fi
		@echo "\n${GREEN}>>> Compilation of miniRT succeded <<<${END}"

$(LIBFT):	$(LIBFT_DIR)
				@$(MAKE) -C $(LIBFT_DIR)
				@echo "\n${GREEN}>>> Compilation of libft succeded <<<${END}"

$(MLX):	$(MLX_DIR)
				@$(MAKE) -C $(MLX_DIR)

clean : 
		$(RM) $(OBJS)
		@$(MAKE) -C $(LIBFT_DIR) clean
		@$(MAKE) -C $(MLX_DIR) clean
		@echo "\n${GREEN}>>> Cleaned <<<${END}"

fclean : clean
		$(RM) $(NAME)
		@if [ $(shell uname) = Darwin ]; then\
			$(RM) $(MLX_NAME);\
		fi
		@$(MAKE) -C $(LIBFT_DIR) fclean
		@$(MAKE) -C $(MLX_DIR) clean
		@echo "\n${GREEN}>>> Fully Cleaned <<<${END}"

re : clean all