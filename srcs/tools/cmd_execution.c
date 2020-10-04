#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include <sys/wait.h>

int		is_our_command(t_command *cmd, t_data *data)
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

int		our_command(t_command *cmd, t_data *data)
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

static void external_func_add(t_command *cmd, t_data *data)
{
	if (cmd->redirect)
	{
		if (check_fd(cmd, data, 1))
			exit(1);
	}
	if (cmd->next && cmd->redirect == 3 && !check_redirect(cmd) \
	&& check_for_pipe(cmd, 0))
	{
		empty_pipe_handler(cmd, data);
		exit(data->errcode);
	}
	execve(cmd->argv[0], cmd->argv, data->envp);
}

int		external_func(t_command *cmd, t_data *data)
{
    int status;
    pid_t pid;

    if ((pid = fork()) < 0)
        return (1);
    if (pid == 0)
    {
		external_func_add(cmd, data);
        exit(0);
    }
    waitpid(pid, &status, 0);
    data->errcode = f_get_exitcode(status);
    if (cmd->redirect && !check_for_pipe(cmd, 1) \
    && check_for_pipe(cmd,0))
    {
        after_redirect_pipe_h(cmd, data);
    }
    return (0);
}
