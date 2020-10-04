/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_caller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 14:41:13 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 14:11:10 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "libft.h"
#include "string.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int		check_fd(t_command *cmd, t_data *data, int type)
{
	t_command *tmp;

	tmp = cmd;
	if (tmp->redirect)
	{
		while (tmp)
		{
			if (fd_handler(tmp, data, type))
				return (1);
			write_in_file(cmd, tmp);
			if (tmp->next->redirect)
			{
				tmp = tmp->next;
				continue ;
			}
			return (0);
		}
	}
	return (0);
}

void		process_handler(t_command *cmd, t_data *data, int kind)
{
	dup2(data->fd[kind], kind);
	close(data->fd[0]);
	close(data->fd[1]);
	if (kind == 1)
		cmd->pipe = 0;
	cmd_caller(data, cmd);
	exit(data->errcode);
}


int		execute_one(t_command *cmd, t_data *data)
{

	data->fd_f = 1;
	if (!is_our_command(cmd, data))
	{
		if (cmd->redirect && cmd->flag2 == 2)
			return (0);
		if (our_command(cmd, data))
			return (1);
		if (cmd->next && cmd->redirect &&\
		check_for_pipe(cmd, 0))
			after_redirect_pipe_h(cmd, data);
		return (0);
	}
	return (external_func(cmd, data));
}


static int		execute(t_command *cmd, t_data *data)
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
		process_handler(cmd->next, data, 0);
	close(data->fd[0]);
	close(data->fd[1]);
	waitpid(pid_1, &status_1, 0);
	waitpid(pid_2, &status_2, 0);
	data->errcode = f_get_exitcode(status_2);
	return (1);
}

int				cmd_caller(t_data *data, t_command *cmd)
{
	data->counter = 0;
	if (cmd->pipe == 0)
		return (execute_one(cmd, data));
	while (cmd)
	{
		data->counter++;
		if (execute(cmd, data))
			return (0);
		cmd = cmd->next;
	}
	return (0);
}
