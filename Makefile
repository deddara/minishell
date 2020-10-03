# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/19 22:18:35 by awerebea          #+#    #+#              #
#    Updated: 2020/10/03 10:42:58 by awerebea         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
LIBFT		= libft/libft.a
INCLUDES	= -I includes/ -I libft/includes/
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror
OFLAGS		= -O2
DBGFLAGS	= -g

override FLAGS ?= $(CFLAGS)

#------------------------------- minishell compiling ---------------------------
SRCDIR		= srcs/
OBJDIR		= objs/

FLSDIR_1	= ./
FLS_1		= $(addprefix $(FLSDIR_1), \
				main )

FLSDIR_2	= tools/
FLS_2		= $(addprefix $(FLSDIR_2), \
				cmd_caller \
				coammand_handler \
				command_checker \
				readline \
				signals \
				strarr \
				structer \
				utils )

FLSDIR_3	= parser/
FLS_3		= $(addprefix $(FLSDIR_3), \
				checks \
				dollar \
				dollar_pars_prepare \
				input_validator \
				parser \
				quotes \
				slash_n_process )

FLSDIR_4	= builtins/
FLS_4		= $(addprefix $(FLSDIR_4), \
				cd \
				echo \
				env \
				exit \
				export \
				pwd \
				unset )

SRC	= $(FLS_1) $(FLS_2) $(FLS_3) $(FLS_4)

OBJ	= $(addprefix $(OBJDIR), $(SRC:=.o))
DFLS	= $(SRC:=.d)

all:		$(NAME)

$(NAME):	$(LIBFT) $(OBJ)
	@echo '---------- Making minishell ----------'
	$(CC)		$(FLAGS) $(OBJ) $(INCLUDES) -L libft -lft -o $(NAME)
	@echo '------------- All done! --------------'

$(OBJ):		$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p	$(OBJDIR) $(addprefix $(OBJDIR), $(FLSDIR_1) $(FLSDIR_2) \
			$(FLSDIR_3) $(FLSDIR_4))
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
	# valgrind --leak-check=full --show-leak-kinds=all -s ./$(NAME)
	./$(NAME)

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
