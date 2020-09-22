/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 12:57:17 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 16:11:48 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	vremennoe_reshenie(t_data *data)
{
	data->argv = malloc(sizeof(char*) * 1);
	data->argv[0] = "/bin/ls";
}

int	f_ls(t_data *data, int fd)
{
	int pid;

	fd = 0;
	if ((pid = fork()) < 0)
		return (errno);
	if (pid == 0)
	{
		vremennoe_reshenie(data);
		execve("/bin/ls", data->argv, data->envp);
	}
	else
	{
		wait(NULL);
		return (0);
	}
	return (0);
}