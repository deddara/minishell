/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_caller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 14:41:13 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/02 17:09:19 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "structer.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "libft.h"
#include "string.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

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

static void		write_in_file(t_command *command, t_command *tmp)
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

static int		fd_handler(t_command *tmp, t_data *data, int type)
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

static int		check_fd(t_command *cmd, t_data *data, int type)
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

static int		is_our_command(t_command *cmd, t_data *data)
{
	if (!ft_strncmp(cmd->argv[0], "pwd", 3))
		return (0);
	else if (!data->counter && !ft_strncmp(cmd->argv[0], "cd", 3))
		return (0);
	else if (!ft_strncmp(cmd->argv[0], "echo", 4))
		return (0);
	else if (!ft_strncmp(cmd->argv[0], "env", 3))
		return (0);
	else if (!ft_strncmp(cmd->argv[0], "export", 6))
		return (0);
	else if (!ft_strncmp(cmd->argv[0], "unset", 5))
		return (0);
	else if (!ft_strncmp(cmd->argv[0], "exit", 4))
		return (0);
	else
		return (1);
}

static int		our_command(t_command *cmd, t_data *data)
{
	data->fd_f = 1;
	if (check_fd(cmd, data, 0))
		return (1);
	if (!ft_strncmp(cmd->argv[0], "pwd", 3))
		f_pwd(data->fd_f);
	else if (!data->counter && !ft_strncmp(cmd->argv[0], "cd", 3))
		f_cd(cmd->argv[1], data);
	else if (!ft_strncmp(cmd->argv[0], "echo", 4))
		f_echo(&cmd->argv[1], data->fd_f);
	else if (!ft_strncmp(cmd->argv[0], "env", 3))
		f_env(data->fd_f, data);
	else if (!ft_strncmp(cmd->argv[0], "export", 6))
		f_export(data, &cmd->argv[1], data->fd_f);
	else if (!ft_strncmp(cmd->argv[0], "unset", 5))
		f_unset(data, &cmd->argv[1]);
	else if (!ft_strncmp(cmd->argv[0], "exit", 4))
		f_exit(data, &cmd->argv[0]);
	return (0);
}

static int		check_for_pipe(t_command **cmd)
{
	while (*cmd)
	{
		if ((*cmd)->pipe)
			return (1);
		(*cmd) = (*cmd)->next;
	}
	return (0);
}

static void		after_redirect_pipe_h(t_command *cmd, t_data *data)
{
	int pid2;
	int status;

	if (cmd->next && cmd->redirect && check_for_pipe(&cmd))
	{
		pid2 = fork();
		if (pid2 == 0)
			cmd_caller(data, cmd);
		waitpid(pid2, &status, 0);
		data->errcode = f_get_exitcode(status);
	}
}

static int		external_func(t_command *cmd, t_data *data)
{
	int status;
	int pid;

	if ((pid = fork()) < 0)
		return (1);
	if (pid == 0)
	{
		if (cmd->redirect)
		{
			if (check_fd(cmd, data, 1))
				exit(1);
		}
		execve(cmd->argv[0], cmd->argv, data->envp);
		exit(0);
	}
	waitpid(pid, &status, 0);
	data->errcode = f_get_exitcode(status);
	after_redirect_pipe_h(cmd, data);
	return (0);
}

static int		execute_one(t_command *cmd, t_data *data)
{
	pid_t	pid2;
	int		status;

	data->fd_f = 1;
	if (!is_our_command(cmd, data))
	{
		if (cmd->redirect && cmd->flag2 == 2)
			return (0);
		if (our_command(cmd, data))
			return (1);
		if (cmd->next && cmd->redirect &&\
		check_for_pipe(&cmd))
		{
			pid2 = fork();
			if (pid2 == 0)
				cmd_caller(data, cmd);
			waitpid(pid2, &status, 0);
			data->errcode = f_get_exitcode(status);
		}
		return (0);
	}
	return (external_func(cmd, data));
}

static void		process_handler(t_command *cmd, t_data *data, int kind)
{
	dup2(data->fd[kind], kind);
	close(data->fd[0]);
	close(data->fd[1]);
	if (kind == 1)
		cmd->pipe = 0;
	cmd_caller(data, cmd);
	exit(data->errcode);
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
