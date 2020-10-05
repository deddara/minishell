/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 11:59:29 by deddara           #+#    #+#             */
/*   Updated: 2020/10/05 11:59:30 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include "libft.h"
#include <stdlib.h>
#include "string.h"
#include <errno.h>
#include <unistd.h>

static int		redirect_in(t_command *cmd, t_data *data, int type)
{
	int			errcode;

	if ((data->fd_in = open(cmd->next->argv[0], O_RDONLY)) < 0)
	{
		errcode = errno;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->next->argv[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errcode), 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
	if (type)
	{
		dup2(data->fd_in, 0);
		close(data->fd_in);
	}
	return (0);
}

static int		redirect_out(t_command *cmd, t_data *data, int type)
{
	if ((data->fd_f = open(cmd->next->argv[0], O_CREAT | O_TRUNC\
	| O_WRONLY, 0644)) < 0)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
	if (type)
	{
		dup2(data->fd_f, 1);
		close(data->fd_f);
	}
	return (0);
}

static int		double_redirect_out(t_command *cmd, t_data *data, int type)
{
	if ((data->fd_f = open(cmd->next->argv[0], O_CREAT | O_APPEND\
	| O_WRONLY, 0644)) < 0)
	{
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		return (1);
	}
	if (type)
	{
		dup2(data->fd_f, 1);
		close(data->fd_f);
	}
	return (0);
}

void			write_in_file(t_command *command, t_command *tmp)
{
	int i;

	i = 1;
	while (tmp->next->argv[i])
	{
		if (!(command->argv = f_strarr_add_elem(command->argv,\
		tmp->next->argv[i])))
			return ;
		i++;
	}
}

int				fd_handler(t_command *tmp, t_data *data, int type)
{
	if (tmp->redirect == 1)
	{
		if (redirect_out(tmp, data, type))
			return (1);
	}
	else if (tmp->redirect == 3)
	{
		if (redirect_in(tmp, data, type))
			return (1);
	}
	else if (tmp->redirect == 2)
	{
		if (double_redirect_out(tmp, data, type))
			return (1);
	}
	return (0);
}
