/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 12:14:32 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/01 12:52:47 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H
# include "structer.h"

typedef struct	s_data
{
	char		**minishell_argv;
	char		**envp;
	char		*input;
	char		**inp_arr;
	int			pos;
	int			start;
	int			qt1_o;
	int			qt1_c;
	int			qt2_o;
	int			qt2_c;
	int			pars_complete;
	char		*w;
	int			last_saved;
	char		*errstr;
	char        **argv;
	int			errcode;
	int			slash;
	int			counter;
	int 		fd[2];
	int 		fd_f;
	int 		fd_in;
	int 		cd;
	int			sig;
}				t_data;

char			**f_strarr_dup(char **src_arr);
char			**f_strarr_add_elem(char **src_arr, char *str);
char			**f_strarr_rem_elem(char **src_arr, char *str, char *endcmp);
int				f_strarr_find_elem(char **src_arr, char *str, char *endcmp);
void			*f_strarr_free(char **strarr);
int				f_quit(t_data *data, int exitcode, char *exitstr);

int				test_strarr(t_data *data);
int				f_cd(char *path, t_data *data);
char			*f_env_find_elem(char **src_arr, char *str, char *endcmp);
void			f_pwd(int fd);
int		        f_unset(t_data *data, char **argv);
void			f_env(int fd, t_data *data);
void			f_echo(char **argv, int fd);
int			    f_export(t_data *data, char **argv, int fd);
int				f_pars_input(t_data *data);
int				structer(t_data *data, t_command *cmd);
void			clear_list(t_command *cmd);
int             check_command(t_data *data, t_command *cmd);
int				command_handler(t_data *data, t_command *cmd);

int				f_pars_input(t_data *data);
int				f_add_segment(t_data *data, int i);
int				f_join_to_w(t_data *data, char *str);
int				f_chk_shield (t_data *data, int i);
int				f_chk_shield_rev(t_data *data, int i);
int				f_chk_quotes(t_data *data, int i);
int				f_word_chks(t_data *data, int *i);
int				f_chk_unclosed_quotes(t_data *data);
int				f_quotes_pars(t_data *data, int *i);
int				f_quote_status(t_data *data);
void			f_clear_quotes_flags(t_data *data);
int				f_slash_pars(t_data *data, int *i);
int				f_process_pars(t_data *data, int *i);
int				f_dollar_pars(t_data *data, int *i);
int				f_dollar_pars_prepare(t_data *data, int *i, int *k);
int				cmd_caller(t_data *data, t_command *cmd);
int				f_readline(char **input);
int				f_exit(t_data *data, t_command *cmd);

#endif
