/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 12:14:32 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/04 17:34:53 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

typedef struct	s_data
{
	char				**minishell_argv;
	char				**envp;
	char				*input;
	char				**inp_arr;
	int					pos;
	int					start;
	int					qt1_o;
	int					qt1_c;
	int					qt2_o;
	int					qt2_c;
	int					pars_complete;
	char				*w;
	int					last_saved;
	char				**argv;
	int					errcode;
	int					slash;
	int					counter;
	int					fd[2];
	int					fd_f;
	int					fd_in;
	int					cd;
	int					sig;
	int					read_started;
	char				*arr;
	int					arr_size;
	int					in_fd_exst;
}				t_data;

typedef struct	s_command
{
	char				**argv;
	int					pipe;
	int					redirect;
	int					file;
	int					flag;
	int					flag2;
	struct s_command	*next;
}				t_command;

extern int		g_read_started;
extern int		g_sigquit;
extern int		g_sigint;

int				f_quit(t_data *data, int exitcode, char *exitstr);
int				f_strarr_find_elem(char **src_arr, char *str, char *endcmp);
char			**f_strarr_add_elem(char **src_arr, char *str);
char			**f_strarr_dup(char **src_arr);
void			*f_strarr_free(char **strarr);
char			**f_strarr_rem_elem(char **src_arr, char *str, char *endcmp);

int				f_cd(char *path, t_data *data);
void			f_echo(char **argv, int fd);
void			f_pwd(int fd);
void			f_env(int fd, t_data *data);
int				f_exit(t_data *data, char **argv);
int				f_export_add_var(t_data *data, char **argv);
int				f_export(t_data *data, char **argv, int fd);
int				f_unset(t_data *data, char **argv);
char			*f_env_find_elem(char **src_arr, char *str, char *endcmp);

int				check_command(t_data *data, t_command *cmd);
void			clear_list(t_command *cmd);
int				cmd_caller(t_data *data, t_command *cmd);
int				command_handler(t_data *data, t_command *cmd);
int				structer(t_data *data, t_command *cmd);
t_command		*create_command_lst(void);

int				f_add_segment(t_data *data, int i);
int				f_chk_quotes(t_data *data, int i);
int				f_chk_shield_rev(t_data *data, int i);
int				f_chk_shield (t_data *data, int i);
int				f_chk_unclosed_quotes(t_data *data);
void			f_clear_input_data(t_data *data);
void			f_clear_quotes_flags(t_data *data);
int				f_dollar_pars_prepare(t_data *data, int *i, int *k);
int				f_dollar_pars(t_data *data, int *i);
int				f_ind_arr_init(t_data *data);
int				f_input_validator(t_data *data);
int				f_join_to_w(t_data *data, char *str);
void			f_line_pos_shift(t_data *data, int *i);
int				f_pars_input(t_data *data);
int				f_process_pars(t_data *data, int *i, int *w_ind);
int				f_quotes_pars(t_data *data, int *i);
int				f_quote_status(t_data *data);
int				f_slash_pars(t_data *data, int *i);
int				f_word_chks(t_data *data, int *i);

int				f_coincidence_char_ind(char *str, char c);
int				f_get_exitcode(int status);
int				f_readline(char **input);
void			f_sigint(void);
void			f_sigquit(void);
int				execute_one(t_command *cmd, t_data *data);
#endif
