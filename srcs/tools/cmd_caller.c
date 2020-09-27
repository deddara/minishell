#include "minishell.h"
#include "structer.h"
#include <unistd.h>
#include <stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
//static int	execute(t_command *cmd, t_command *prev, t_data *data)
//{
//	int pid;
//	int pid2;
//
//	if (data->counter != 0 && prev->pipe == 1)
//	{
//		pid2 = fork();
//		if (pid2 < 0)
//			return (1);
//		if (pid2 == 0)
//		{
//			dup2(data->fd[0], STDIN_FILENO);
//			close(data->fd[0]);
//			close(data->fd[1]);
//			execve(cmd->argv[0], cmd->argv, data->envp);
//		}
//	}
//	if (cmd->pipe == 1)
//	{
//		if (pipe(data->fd) == -1)
//			return (1);
//		pid = fork();
//		if (pid < 0)
//			return (1);
//		if (pid == 0)
//		{
//			dup2(data->fd[1], STDOUT_FILENO);
//			close(data->fd[1]);
//			execve(cmd->argv[0], cmd->argv, data->envp);
//		}
//		data->counter++;
//		close(data->fd[1]);
//		close(data->fd[0]);
//		wait(NULL);
//		return (0);
//	}
//	else
//	{
//		pid = fork();
//		if (pid < 0)
//			return (1);
//		if (pid == 0)
//		{
//			execve(cmd->argv[0], cmd->argv, data->envp);
//		}
//	}
//	wait(NULL);
//	return (0);
//}

static int execute_one(t_command *cmd, t_data *data)
{
    pid_t   pid;
    int     status;

    if ((pid = fork()) < 0)
        return (1);
    if (pid == 0)
    {
        execve(cmd->argv[0], cmd->argv, data->envp);
    }
    waitpid(pid, &status, 0);
    return (0);
}

static void process_handler(t_command *cmd, t_data *data, int kind)
{
    dup2(data->fd[kind], kind);
    close(data->fd[0]);
    close(data->fd[1]);
    if (kind == 1)
        cmd->pipe = 0;
    cmd_caller(data, cmd);
    exit (0);
}

static int execute(t_command *cmd, t_data *data)
{
    pid_t pid_1;
    pid_t pid_2;
    int     status_1;
    int     status_2;

    if (pipe(data->fd) == 1)
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
		if (execute(cmd, data))
			return (0);
		cmd = cmd->next;
	}
	return (0);
}