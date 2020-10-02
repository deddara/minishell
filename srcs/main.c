/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 00:51:31 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/02 16:29:35 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "structer.h"
#include <signal.h>

int				g_read_started;
int				g_sigquit;

void			f_data_init(t_data *data, char **argv)
{
	data->minishell_argv = argv;
	data->envp = NULL;
	data->input = NULL;
	data->inp_arr = NULL;
	data->pos = 0;
	data->start = 0;
	data->qt1_o = 0;
	data->qt1_c = 0;
	data->qt2_o = 0;
	data->qt2_c = 0;
	data->cd = 0;
	data->pars_complete = 0;
	data->w = NULL;
	data->last_saved = 0;
	data->errstr = NULL;
	data->errcode = 0;
	data->slash = 0;
	data->sig = 0;
}

int				f_quit(t_data *data, int exitcode, char *exitstr)
{
	data->envp = f_strarr_free(data->envp);
	data->inp_arr = f_strarr_free(data->inp_arr);
	free((data->input) ? data->input : NULL);
	data->input = NULL;
	free((data->w) ? data->w : NULL);
	data->w = NULL;
	ft_putstr_fd(exitstr, (exitcode) ? 2 : 1);
	free((data->errstr) ? data->errstr : NULL);
	data->errstr = NULL;
	return (exitcode);
}

void			f_clear_input_data(t_data *data)
{
	data->pars_complete = 0;
	data->inp_arr = f_strarr_free(data->inp_arr);
	free((data->w) ? data->w : NULL);
	data->w = NULL;
	free((data->errstr) ? data->errstr : NULL);
	data->errstr = NULL;
	data->pos = 0;
	data->start = 0;
	data->qt1_o = 0;
	data->qt1_c = 0;
	data->qt2_o = 0;
	data->qt2_c = 0;
	data->pars_complete = 0;
	data->last_saved = 0;
	data->slash = 0;
	data->sig = 0;
}

int				main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_command	*command;

	command = NULL;
	signal(SIGINT, (void*)f_sigint);
	signal(SIGQUIT, (void*)f_sigquit);
	(void)argc;
	f_data_init(&data, argv);
	if (!(data.envp = f_strarr_dup(envp)))
		return (f_quit(&data, 1, "malloc error\n"));
	data.input = ft_strdup("start :)");
	while (1)
	{
		g_read_started = 1;
				ft_putstr_fd("minishell$ ", 1);
		free((data.input) ? data.input : NULL);
		data.input = NULL;
		g_sigquit = 0;
		if (f_readline(&data.input))
			return (f_quit(&data, 0, ""));
		while (!data.pars_complete)
		{
			if (!(command = create_command_lst()))
				return (1);
			if ((data.errcode = f_pars_input(&data)))
			{
				ft_putstr_fd(data.errstr, 2);
				break;
			}
			if ((data.errcode = structer(&data, command)))
				continue;
			if ((data.errcode = command_handler(&data, command)))
			{
				g_read_started = 0;
				clear_list(command);
				continue;
			}
			g_read_started = 0;
			cmd_caller(&data, command);
			clear_list(command);
		}
		f_clear_input_data(&data);
	}
	return (f_quit(&data, 0, ""));
}
