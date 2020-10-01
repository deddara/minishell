/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_strarr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 00:51:31 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/30 17:34:24 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

static int		test_strarr_add_elements(t_data *data)
{
	if (!(data->envp = f_strarr_add_elem(data->envp, "test1=1")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_add_elem(data->envp, "test2")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_add_elem(data->envp, "test3")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_add_elem(data->envp, "test4test4")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_add_elem(data->envp, "test5=string5")))
		return (f_quit(data, 1, "malloc error\n"));
	return (0);
}

static int		test_strarr_rem_elements(t_data *data)
{
	if (!(data->envp = f_strarr_rem_elem(data->envp, "test1", "=")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_rem_elem(data->envp, "test2", "=")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_rem_elem(data->envp, "test3", "")))
		return (f_quit(data, 1, "malloc error\n"));
	if (!(data->envp = f_strarr_rem_elem(data->envp, "test4", "qwerty")))
		return (f_quit(data, 1, "malloc error\n"));
	return (0);
}

int				test_strarr(t_data *data)
{
	int			i;

	i = 0;
	if (!(data->envp = f_strarr_add_elem(data->envp, "dsad")))
		return (f_quit(data, 1, "error\n"));
	data->envp = f_strarr_rem_elem(data->envp, "test4", NULL);
	while (data->envp[i])
	{
		ft_putstr_fd(data->envp[i++], 1);
		ft_putchar_fd('\n', 1);
	}
	// test_strarr_rem_elements(data);
	// i = 0;
	// while (data->envp[i])
	// {
	// 	ft_putstr_fd(data->envp[i++], 1);
	// 	ft_putchar_fd('\n', 1);
	// }
	return (f_quit(data, 0, ""));
}
