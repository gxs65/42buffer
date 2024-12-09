PRINTFDIR = printf/
PRINTFBASENAMES = 	ft_printf.c					ft_printf_specnbr.c		ft_printf_utils.c \
					ft_printf_specother.c		ft_put_fd.c				ft_printf_specnbr2.c \
					ft_printf_readspecs.c		ft_printf_specstr.c		ft_put_in.c	\
					ft_printf_others.c
PRINTFSOURCES = $(addprefix $(PRINTFDIR), $(PRINTFBASENAMES))
PRINTFOBJECTS = $(PRINTFSOURCES:.c=.o)

SOURCES = ft_fractol.c ft_complex_ops.c ft_fractol_utils.c ft_mlx_drawings_base.c ft_mlx_drawings_fractal.c
OBJECTS = $(SOURCES:.c=.o)

NAME = fractol
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS) $(PRINTFOBJECTS)
	cc $(CFLAGS) -I. $(OBJECTS) $(PRINTFOBJECTS) -Lmlx -lmlx -lXext -lX11 -lm -lz -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -Imlx -c $< -o $@

re: fclean all

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJECTS) $(PRINTFOBJECTS)
