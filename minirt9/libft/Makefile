SRCS            = $(wildcard srcs/*.c) $(wildcard ft_printf/*.c) $(wildcard gnl/*.c)
OBJS			= ${SRCS:.c=.o}

NAME			= libft.a

CC				= cc
RM				= rm -f
AR				= ar rc
INCLUDES		+= -I includes/

CFLAGS			= -Wall -Wextra -Werror ${INCLUDES}

.c.o:
			${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

$(NAME):	${OBJS}
			${AR} ${NAME} ${OBJS}

all:		$(NAME)

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} $(NAME)

re:			fclean all