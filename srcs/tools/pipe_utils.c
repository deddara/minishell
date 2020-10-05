/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 11:59:27 by deddara           #+#    #+#             */
/*   Updated: 2020/10/05 11:59:28 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int				check_for_pipe(t_command *cmd, int type)
{
	t_command *tmp;

	tmp = cmd;
	if (!type)
	{
		while (tmp)
		{
			if ((tmp)->pipe)
				return (1);
			(tmp) = (tmp)->next;
		}
	}
	else
	{
		while (tmp)
		{
			if ((tmp)->redirect == 3)
				return (1);
			(tmp) = (tmp)->next;
		}
	}
	return (0);
}

void			after_redirect_pipe_h(t_command *cmd, t_data *data)
{
	while (cmd)
	{
		if (cmd->pipe)
		{
			cmd_caller(data, cmd);
			return ;
		}
		cmd = cmd->next;
	}
}

static int		execute2(t_command *cmd, t_command *cmd2, t_data *data)
{
	pid_t		pid_1;
	pid_t		pid_2;
	int			status_1;
	int			status_2;

	if (pipe(data->fd) == -1)
		return (1);
	if ((pid_1 = fork()) < 0)
		return (1);
	if (pid_1 == 0)
		process_handler(cmd, data, 1);
	if ((pid_2 = fork()) < 0)
		return (1);
	if (pid_2 == 0)
		process_handler(cmd2, data, 0);
	close(data->fd[0]);
	close(data->fd[1]);
	waitpid(pid_1, &status_1, 0);
	waitpid(pid_2, &status_2, 0);
	data->errcode = f_get_exitcode(status_2);
	return (1);
}

int				empty_pipe_handler(t_command *cmd, t_data *data)
{
	t_command *tmp;

	tmp = cmd;
	while (tmp)
	{
		if ((tmp)->pipe)
		{
			cmd->redirect = 0;
			execute2(cmd, tmp->next, data);
			return (0);
		}
		(tmp) = (tmp)->next;
	}
	return (0);
}

int				check_redirect(t_command *cmd)
{
	while (cmd)
	{
		if (cmd->redirect == 1 || cmd->redirect == 2)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
