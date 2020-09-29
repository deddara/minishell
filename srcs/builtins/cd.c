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

static void old_pwd(t_data *data)
{
	char	*old_p;
	char 	*tmp;

	if (!(old_p = getcwd(NULL, 0)))
		return ;
	if (!f_env_find_elem(data->envp, "OLDPWD", "="))
		return ;
	data->envp = f_strarr_rem_elem(data->envp, "OLDPWD", "=");
	tmp = old_p;
	old_p = ft_strjoin("OLDPWD=", old_p);
	data->envp = f_strarr_add_elem(data->envp, old_p);
	free(tmp);
	free(old_p);
}

static void new_pwd(t_data *data)
{
	char *new_p;
	char *tmp;

	if (!(new_p = getcwd(NULL, 0)))
		return ;
	if (!f_env_find_elem(data->envp, "PWD", "="))
		return ;
	data->envp = f_strarr_rem_elem(data->envp, "PWD", "=");
	tmp = new_p;
	new_p = ft_strjoin("PWD=", new_p);
	data->envp = f_strarr_add_elem(data->envp, new_p);
	free(tmp);
	free(new_p);
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

static int	tilda_handle(char *path, t_data *data)
{
	char *new_path;
	char *home;

	if (!(home = f_env_find_elem(data->envp, "HOME", "=")))
	{
		old_pwd(data);
		if (chdir(&path[1]) == -1)
			return (path_error(path));
		new_pwd(data);
		return (0);
	}
	if (!(new_path = ft_strdup(home)))
		return (1);
	if (!(new_path = ft_strjoin(new_path, &path[1])))
		return (1);
	old_pwd(data);
	if (chdir(new_path) == -1)
		return (path_error(new_path));
	new_pwd(data);
	free(new_path);
	return (0);
}

int			f_cd(char *path, t_data *data)
{
	char *home;

	if (!path)
	{
		if (!(home = f_env_find_elem(data->envp, "HOME", "=")))
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (errno);
		}
		old_pwd(data);
		if (chdir(home) == -1)
			return (path_error(home));
		new_pwd(data);
		return (0);
	}
	else if (path[0] == '~')
		return (tilda_handle(path, data));
	old_pwd(data);
	if (chdir(path) == -1)
		return (path_error(path));
	new_pwd(data);
	return (0);
}
