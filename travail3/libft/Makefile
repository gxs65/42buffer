# *************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilevy <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/08 18:10:43 by ilevy             #+#    #+#              #
#    Updated: 2024/11/30 21:46:07 by ilevy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#General Purpose

NAME:=libft.a
INCLUDE:= h_files
CC:= gcc
CFLAGS:= -Wall -Wextra -Werror -I
SRC_DIR:= src/
OBJ_DIR:= obj/
#Source_dirs and their contents

FTIS_DIR:= ft_is/
FTIS = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isprint.c ft_isdigit.c ft_iswhitespace.c

FTTO_DIR:= ft_to/
FTTO:= ft_atoi.c ft_itoa.c ft_tolower.c ft_toupper.c

FTMEM_DIR:= ft_mem/
FTMEM:= ft_bzero.c ft_calloc.c ft_memset.c ft_memcmp.c ft_memchr.c ft_memmove.c

FTSTR_DIR:= ft_str/
FTSTR:= ft_split.c ft_strchr.c ft_strdup.c ft_strncmp.c ft_strjoin.c ft_striteri.c\
	ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strnstr.c ft_substr.c\
	ft_strrchr.c ft_strtrim.c

FTLST_DIR:= ft_lst/
FTLST:= ft_lstadd_back_bonus.c ft_lstadd_front_bonus.c ft_lstiter_bonus.c\
       	ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstlast_bonus.c\
	ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstsize_bonus.c\

FTPUT_DIR:= ft_put/
FTPUT:= ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c

# ft_printf somewhat different from the rest of libft, because from abedin
# to have a more async-safe version
FTPRINTF_DIR:= ft_printf/
FTPRINTF:=  ft_printf_main.c		ft_printf_main_count.c		ft_put_in.c \
			ft_printf_utils1.c		ft_printf_utils2.c			ft_printf_readspecs.c \
			ft_printf_specstr.c		ft_printf_specstr_count.c \
			ft_printf_specother.c	ft_printf_specother_count.c \
			ft_printf_specnbr.c		ft_printf_specnbr_count.c	ft_printf_specnbr_extras.c

FTGETNEXT_DIR:= ft_getnext/
FTGETNEXT:= ft_get_next_line.c ft_get_next_line_utils.c

# Source_files relative to src/

SRC_FILES:= $(addprefix $(FTIS_DIR), $(FTIS))
SRC_FILES+= $(addprefix $(FTMEM_DIR), $(FTMEM))
SRC_FILES+= $(addprefix $(FTPUT_DIR), $(FTPUT))
SRC_FILES+= $(addprefix $(FTTO_DIR), $(FTTO))
SRC_FILES+= $(addprefix $(FTLST_DIR), $(FTLST))
SRC_FILES+= $(addprefix $(FTSTR_DIR), $(FTSTR))
SRC_FILES+= $(addprefix $(FTPRINTF_DIR), $(FTPRINTF))
SRC_FILES+= $(addprefix $(FTGETNEXT_DIR), $(FTGETNEXT))

# Source_files and Object_files relative to Makefile

SRC:= $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ:= $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

# Commands to give.

all:	$(NAME)

$(NAME):	$(OBJ)
	ar -rcs $(NAME) $(OBJ)
	@echo "Libft is compiled!"

objdir_mk:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)$(FTIS_DIR)
	@mkdir -p $(OBJ_DIR)$(FTSTR_DIR)
	@mkdir -p $(OBJ_DIR)$(FTPUT_DIR)
	@mkdir -p $(OBJ_DIR)$(FTTO_DIR)
	@mkdir -p $(OBJ_DIR)$(FTLST_DIR)
	@mkdir -p $(OBJ_DIR)$(FTMEM_DIR)
	@mkdir -p $(OBJ_DIR)$(FTPRINTF_DIR)
	@mkdir -p $(OBJ_DIR)$(FTGETNEXT_DIR)
	@echo "Created the directories for the object files!"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c | objdir_mk
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@echo ".o files have been removed!"

fclean:	clean
	rm -rf $(NAME)
	@echo "removed Libft.a!"

re:	fclean all
	@echo "Let's start over fresh!"

.PHONY: all clean fclean re
