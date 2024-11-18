SOURCES = ft_printf_chars.c ft_printf_nbrs.c ft_put.c ft_printf.c
OBJECTS = $(SOURCES:.c=.o)
NAME = libftprintf.a
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS)
	ar -rcs $(NAME) $(OBJECTS)

bonus: $(OBJECTS) $(BOBJECTS)
	ar -rcs $(NAME) $(OBJECTS) $(BOBJECTS)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

re: fclean all

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJECTS) $(BOBJECTS)

tests:
	cp ../main2.c .
	cc $(CFLAGS) main2.c ft_printf.c ft_put.c -o testing
	- ./testing
	rm -f main2.c *.o testing
