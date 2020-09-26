/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:18:54 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 22:07:28 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

static void	change_pwd(char *pwd, char **envp)
{
	char	*old_pwd;
	char	*new_pwd;
	int		flag;

	flag = -1;
	if (!(old_pwd = f_env_find_elem(envp, "PWD", "=")))
		flag = 1;
	if (!f_env_find_elem(envp, "OLDPWD", "="))
		flag++;
	envp = f_strarr_rem_elem(envp, "PWD", "=");
	envp = f_strarr_rem_elem(envp, "OLDPWD", "=");
	if (flag == 2)
		return;
	else if (flag == 1)
	{
		old_pwd = ft_strjoin("OLDPWD=", old_pwd);
		envp = f_strarr_add_elem(envp, old_pwd);
		free (old_pwd);
	}
	else if (flag == 0)
	{
		new_pwd = ft_strjoin("PWD=", pwd);
		envp = f_strarr_add_elem(envp, new_pwd);
		free(new_pwd);
	}
	else if (flag == -1)
	{
		old_pwd = ft_strjoin("OLDPWD=", old_pwd);
		envp = f_strarr_add_elem(envp, old_pwd);
		free (old_pwd);
		new_pwd = ft_strjoin("PWD=", pwd);
		envp = f_strarr_add_elem(envp, new_pwd);
		free(new_pwd);
	}
}

static int	path_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
	return (errno);
}

char		*f_env_find_elem(char **src_arr, char *str, char *endcmp)
{
	int			i;
	int			len;
	char		*coincidence;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	i = -1;
	while (src_arr[++i])
	{
		if ((coincidence = ft_strnstr(src_arr[i], str, len)) && \
			coincidence == src_arr[i] && (ft_strchr(endcmp, src_arr[i][len]) \
			&& !(!src_arr[i][len] && ft_strlen(endcmp))))
		{
			while (*coincidence != '=')
				coincidence++;
			coincidence++;
			return (coincidence);
		}
	}
	return (NULL);
}

static int	tilda_handle(char *path, char **envp)
{
	char *new_path;
	char *home;

	if (!(home = f_env_find_elem(envp, "HOME", "=")))
	{
		if (chdir(&path[1]) == -1)
			return (path_error(path));
		change_pwd(path, envp);
		return (0);
	}
	if (!(new_path = ft_strdup(home)))
		return (1);
	if (!(new_path = ft_strjoin(new_path, &path[1])))
		return (1);
	if (chdir(new_path) == -1)
		return (path_error(new_path));
	change_pwd(new_path, envp);
	free(new_path);
	return (0);
}

int			f_cd(char *path, char **envp)
{
	char *home;

	if (!path)
	{
		if (!(home = f_env_find_elem(envp, "HOME", "=")))
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (errno);
		}
		if (chdir(home) == -1)
			return (path_error(home));
		change_pwd(home, envp);
		return (0);
	}
	else if (path[0] == '~')
		return (tilda_handle(path, envp));
	if (chdir(path) == -1)
	{
		return (path_error(path));
	}
	change_pwd(path, envp);
	return (0);
}
