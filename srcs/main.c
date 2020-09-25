/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 00:51:31 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/24 18:49:39 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "structer.h"

void			f_data_init(t_data *data)
{
	data->envp = NULL;
	data->input = NULL;
	data->inp_arr = NULL;
	data->pos = 0;
	data->qt1_o = 0;
	data->qt1_c = 0;
	data->qt2_o = 0;
	data->qt2_c = 0;
	data->pars_complete = 0;
	data->w = NULL;
	data->last_saved = 0;
	data->errstr = NULL;
	data->slash = 0;
}

int				f_exit(t_data *data, int exitcode, char *exitstr)
{
	data->envp = f_strarr_free(data->envp);
	free((data->input) ? data->input : NULL);
	data->input = NULL;
	ft_putstr_fd(exitstr, (exitcode) ? 2 : 1);
	free((data->errstr) ? data->errstr : NULL);
	data->errstr = NULL;
	return (exitcode);
}

int				main(int argc, char **argv, char **envp)
{
	t_data		data;
	t_command	*command;

	command = NULL;
	(void)argv;
	(void)argc;
	f_data_init(&data);
	if (!(data.envp = f_strarr_dup(envp)))
		return (f_exit(&data, 1, "malloc error\n"));
	if (!(command = create_command_lst()))
			return (1);
	get_next_line(0, &data.input);
	if (f_pars_input(&data))
		return (f_exit(&data, 1, data.errstr));
	//	while (data.inp_arr[i])
	//	{
	//		ft_putstr_fd(data.inp_arr[i++], 1);
	//		ft_putchar_fd('\n', 1);
	//	}
	write (1, "da", 2);
	if (structer(&data, command))
		return (1);
	clear_list(command);
	/* ft_putstr_fd(data.input, 1); */
	/* ft_putchar_fd('\n', 1);      */
	return (f_exit(&data, 0, ""));
}
