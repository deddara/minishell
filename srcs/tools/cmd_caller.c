/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_caller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 14:41:13 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/29 14:41:19 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "structer.h"
#include <unistd.h>
#include <stdlib.h>
#include<stdio.h>
#include "libft.h"
#include "string.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

static int check_fd(t_command *cmd, t_data *data)
{
	if (cmd->redirect == 1)
	{
		while (cmd)
		{
			if ((data->fd_f = open(cmd->next->argv[0], O_RDWR | O_CREAT, 0755)) < 0)
				return (1);
			if (cmd->next->redirect)
			{
				cmd = cmd->next;
				continue ;
			}
			dup2(data->fd_f, 1);
			close(data->fd_f);
			return (0);
		}
	}
	return (0);
}

static int our_command(t_command *cmd, t_data *data)
{
	if ((check_fd(cmd, data)))
		return (1);
	if (!ft_strncmp(cmd->argv[0], "pwd", 3))
		f_pwd(1);
	else if (!data->counter && !ft_strncmp(cmd->argv[0], "cd", 2))
		f_cd(cmd->argv[1], data);
	else if (!ft_strncmp(cmd->argv[0], "echo", 4))
		f_echo(&cmd->argv[1], 1);
	else if (!ft_strncmp(cmd->argv[0], "env", 3))
		f_env(1, data);
	else if (!ft_strncmp(cmd->argv[0], "export", 6))
		f_export(data, &cmd->argv[1], 1);
	else if (!ft_strncmp(cmd->argv[0], "unset", 5))
		f_unset(data, &cmd->argv[1]);
	else
		return (1);
	return (0);
}



static int		execute_one(t_command *cmd, t_data *data)
{
	pid_t	pid;
	int		status;


	if ((pid = fork()) < 0)
		return (1);
	if (pid == 0)
	{
		if (!our_command(cmd, data))
		{
			exit (0);
		}
		if ((check_fd(cmd, data)))
			return (1);
		execve(cmd->argv[0], cmd->argv, data->envp);
	}
	waitpid(pid, &status, 0);
	return (0);
}

static void		process_handler(t_command *cmd, t_data *data, int kind)
{
	dup2(data->fd[kind], kind);
	close(data->fd[0]);
	close(data->fd[1]);
	if (kind == 1)
		cmd->pipe = 0;
	cmd_caller(data, cmd);
	exit (0);
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
	return (1);
}

int			cmd_caller(t_data *data, t_command *cmd)
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
