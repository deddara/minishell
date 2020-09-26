/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 00:51:31 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/26 16:45:01 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "structer.h"

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
	data->pars_complete = 0;
	data->w = NULL;
	data->last_saved = 0;
	data->errstr = NULL;
	data->errcode = 0;
	data->slash = 0;
}

int				f_exit(t_data *data, int exitcode, char *exitstr)
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
	data->errcode = 127;
	data->slash = 0;
}

int				main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_command	*command;

	command = NULL;
	(void)argc;
	f_data_init(&data, argv);
	if (!(data.envp = f_strarr_dup(envp)))
		return (f_exit(&data, 1, "malloc error\n"));
	data.input = ft_strdup("start :)");
	while (ft_strncmp("exit", data.input, ft_strlen("exit")))
	{
		ft_putstr_fd("minishell$ ", 1);
		free((data.input) ? data.input : NULL);
		data.input = NULL;
		get_next_line(0, &data.input);
		while (!data.pars_complete)
		{
			if (!(command = create_command_lst()))
				return (1);
			if (f_pars_input(&data))
				return (f_exit(&data, 1, data.errstr));
			if (structer(&data, command))
				return (1);
			command_handler(&data, command);
			clear_list(command);
		}
		f_clear_input_data(&data);
	}
	return (f_exit(&data, 0, ""));
}

//	while (data.inp_arr[i])
//	{
//		ft_putstr_fd(data.inp_arr[i++], 1);
//		ft_putchar_fd('\n', 1);
//	}