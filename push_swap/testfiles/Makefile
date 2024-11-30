PRINTFDIR = printf/
PRINTFBASENAMES = 	ft_printf.c					ft_printf_specnbr.c			ft_printf_utils.c \
					ft_printf_specother.c		ft_put_fd.c					ft_printf_specnbr2.c \
					ft_printf_readspecs.c		ft_printf_specstr.c			ft_put_in.c
PRINTFSOURCES = $(addprefix $(PRINTFDIR), $(PRINTFBASENAMES))
PRINTFOBJECTS = $(PRINTFSOURCES:.c=.o)

BASENAMES = 		ft_push_swap.c				ft_ops_rlist.c				ft_ops_stack.c \
					ft_sort_fcost.c				ft_sort_fcost_utils.c		ft_sort_fcost_initial.c \
					ft_io_display_stacks.c		ft_io_read_integer_list.c	ft_sort_smallstack.c \
					ft_sort_smallstack_utils.c
SOURCES = $(BASENAMES)
OBJECTS = $(SOURCES:.c=.o)

HEADERS =			$(PRINTFDIR)ft_printf.h		ft_push_swap.h

NAME = push_swap
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS) $(PRINTFOBJECTS) $(HEADERS)
	cc $(CFLAGS) $(OBJECTS) $(PRINTFOBJECTS) -o $(NAME)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

re: fclean all

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJECTS) $(PRINTFOBJECTS)

tests: all
	shuf -i 1-10 | tr '\n' ' ' > input
	./push_swap $(cat input) | tr '\n' ' ' > result

names:
	@echo $(SOURCES) $(PRINTFSOURCES)