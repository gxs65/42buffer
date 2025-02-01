# Program Name
NAME:= miniRT

# Flags and Compiler
CC:= cc
CFLAGS:= -g3 -Wall -Wextra -Werror

# Dossiers
C_DIR:= c_files/
H_DIR:= h_files/
O_DIR:= o_files/
SUBS = mlx/ main/ ops/
O_DIR_SUBS = $(addprefix $(O_DIR), $(SUBS))
LIBFT_DIR:= libft/

# C files in isolation
FILES:=
FILES_MAIN = ft_minirt.c 			ft_calc.c
FILES+= $(addprefix main/, $(FILES_MAIN))
FILES_MLX = ft_mlx_colors.c 		ft_mlx_events.c \
			ft_mlx_images.c
FILES+= $(addprefix mlx/, $(FILES_MLX))
FILES_OPS = ft_ops_vec_std.c 		ft_ops_ray_std.c
FILES+= $(addprefix ops/, $(FILES_OPS))

# Files relative to the Makefile
C_FILES:= $(addprefix $(C_DIR), $(FILES))
O_FILES:= $(addprefix $(O_DIR), $(FILES:.c=.o))

# Rules

all:	$(NAME)

$(NAME):	$(LIBFT_DIR)libft.a	$(O_DIR_SUBS)	$(O_FILES)
	$(CC) $(CFLAGS) -I $(H_DIR) -o $@ $(O_FILES) -L $(LIBFT_DIR) -lft -Lmlx -lmlx -lXext -lX11 -lm -lz
	@echo "Compiled miniRT"

$(O_DIR)%.o:	$(C_DIR)%.c
	$(CC) $(CFLAGS) -I $(H_DIR) -Imlx -c $< -o $@

$(O_DIR_SUBS): $(O_DIR)
	@mkdir -p $(O_DIR_SUBS)

$(O_DIR):
	@mkdir -p $(O_DIR)

$(LIBFT_DIR)libft.a:
	make -C $(LIBFT_DIR)

clean:
	make clean -C $(LIBFT_DIR)
	rm -rf $(O_FILES)
	rm -rf $(O_DIR)

fclean:	clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT_DIR)

minilibx:
	git clone https://github.com/42Paris/minilibx-linux mlx/ && cd mlx/ && ./configure && cd ..

bonus: all

re:	fclean all

names:
	echo $(FILES) $(O_DIR_SUBS)

.PHONY: re fclean clean all
