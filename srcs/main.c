/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 00:51:31 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/22 18:58:11 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void			f_data_init(t_data *data)
{
	data->envp = NULL;
	data->input = NULL;
}

int				f_exit(t_data *data, int exitcode, char *exitstr)
{
	data->envp = f_strarr_free(data->envp);
	free((data->input) ? data->input : NULL);
	data->input = NULL;
	ft_putstr_fd(exitstr, (exitcode) ? 2 : 1);
	return (exitcode);
}

int				main(int argc, char **argv, char **envp)
{
	t_data		data;

	(void)argv;
	(void)argc;
	f_data_init(&data);
	if (!(data.envp = f_strarr_dup(envp)))
		return (f_exit(&data, 1, "malloc error\n"));
	get_next_line(0, &data.input);
	ft_putstr_fd(data.input, 1);
	return (f_exit(&data, 0, ""));
}
