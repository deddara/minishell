# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/19 22:18:35 by awerebea          #+#    #+#              #
#    Updated: 2020/09/21 19:59:44 by deddara          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
LIBFT		= libft/libft.a
INCLUDES	= -I includes/ -I libft/includes/
CC		= gcc
CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -O2
DBGFLAGS	= -g

override FLAGS ?= $(CFLAGS)

#------------------------------- minishell compiling ---------------------------
SRCDIR		= srcs/
OBJDIR		= objs/
FLSDIR_1	= ./
FLSDIR_2	= tools/
FLSDIR_3	= tests/

FLS_1	= $(addprefix $(FLSDIR_1), \
		main )

FLS_2	= $(addprefix $(FLSDIR_2), \
		strarr pwd)

FLS_3	= $(addprefix $(FLSDIR_3), \
		 )

SRC	= $(FLS_1) $(FLS_2) $(FLS_3)

OBJ	= $(addprefix $(OBJDIR), $(SRC:=.o))
DFLS	= $(SRC:=.d)

all:		$(NAME)

$(NAME):	$(LIBFT) $(OBJ)
	@echo '---------- Making minishell ----------'
	$(CC)		$(FLAGS) $(OBJ) $(INCLUDES) -L libft -lft -o $(NAME)
	@echo '------------- All done! --------------'

$(OBJ):		$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(FLSDIR_1) $(FLSDIR_2) \
			$(FLSDIR_3))
	$(CC)		$(FLAGS) $(INCLUDES) -c $< -o $@ -MMD

include $(wildcard $(addprefix $(OBJDIR), $(DFLS)))

clean:
	rm -rf	$(OBJDIR)

clean_all: clean libft_clean

fclean:	clean
	rm -f	$(NAME)

fclean_all: fclean libft_fclean

debug:
	make FLAGS="$(CFLAGS) $(DBGFLAGS)" all

$(LIBFT): libft_force_make
	@echo '------------ Making libft ------------'
	@make		-C libft/ --no-print-directory

libft_clean:
	make clean	-C libft/

libft_fclean:
	make fclean	-C libft/

libft_re:
	make re		-C libft/

run: all
	./$(NAME)

test: debug
	valgrind --leak-check=full -s ./$(NAME)

re:	fclean all

.PHONY:	all \
	clean \
	clean_all \
	fclean \
	fclean_all \
	debug \
	libft_clean \
	libft_fclean \
	libft_force_make \
	libft_re \
	run \
	run_debug \
	re
