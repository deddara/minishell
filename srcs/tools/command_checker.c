/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 19:34:14 by deddara           #+#    #+#             */
/*   Updated: 2020/10/02 16:21:07 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <sys/errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static int			check_existence(char *c_path)
{
	if (open(c_path, O_RDONLY) == -1)
	{
		return (1);
	}
	return (0);
}

static int			new_path_create(char *c_path, t_command *command)
{
	if (!ft_strncmp(command->argv[0], "pwd", 3) \
	|| !ft_strncmp(command->argv[0], "echo", 4) ||\
	!ft_strncmp(command->argv[0], "env", 3) ||\
	!ft_strncmp(command->argv[0], "cd", 2))
	{
		free(c_path);
		return (0);
	}
	free(command->argv[0]);
	command->argv[0] = ft_strdup(c_path);
	free(c_path);
	return (0);
}

static int			error_print(t_command *command)
{
	if ((!ft_strncmp(command->argv[0], "cd", 2) && \
	ft_strlen(command->argv[0]) == 2) || \
	(!ft_strncmp(command->argv[0], "exit", 4) \
	&& ft_strlen(command->argv[0]) == 4) || \
	(!ft_strncmp(command->argv[0], "export", 6) \
	&& ft_strlen(command->argv[0]) == 6) || \
		(!ft_strncmp(command->argv[0], "unset", 5) \
		&& ft_strlen(command->argv[0]) == 5))
		return (0);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	return (127);
}

static int			find_command(char **path_data, t_command *command)
{
	int		i;
	char	*tmp;
	char	*c_path;

	c_path = NULL;
	i = 0;
	while (path_data[i])
	{
		if (!(c_path = ft_strjoin(path_data[i], "/")))
			return (1);
		tmp = c_path;
		if (!(c_path = ft_strjoin(c_path, command->argv[0])))
			return (1);
		free(tmp);
		if (!check_existence(c_path))
			return (new_path_create(c_path, command));
		i++;
		free(c_path);
	}
	return (error_print(command));
}

int					check_command(t_data *data, t_command *cmd)
{
	char	*path_p;
	char	**path_data;

	if (cmd->argv[0][0] == '/')
	{
		if (check_existence(cmd->argv[0]))
		{
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putchar_fd('\n', 2);
			return (127);
		}
		return (0);
	}
	if (!(path_p = f_env_find_elem(data->envp, "PATH", "=")))
		return (1);
	if (!(path_data = ft_split(path_p, ':')))
		return (1);
	return (find_command(path_data, cmd));
}
