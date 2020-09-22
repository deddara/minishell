/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:18:54 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 21:45:09 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

static int	path_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
	return (errno);
}

char	*f_env_find_elem(char **src_arr, char *str, char *endcmp)
{
	int			i;
	int			j;
	int			len;
	char		*coincidence;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	i = -1;
	j = 0;
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

static int	tilda_handle(char *path, char**envp)
{
	char *new_path;
	char *home;

	if(!(home = f_env_find_elem(envp, "HOME", "=")))
	{
		if (chdir(&path[1]) == -1)
			return (path_error(path));
		return (0);
	}
	if (!(new_path = ft_strdup(home)))
		return (1);
	if (!(new_path = ft_strjoin(new_path, &path[1])))
		return (1);
	if (chdir(new_path) == -1)
		return (path_error(new_path));
	return (0);
}

int			f_cd(char *path, char **envp)
{
	char *home;

	if (!path)
	{
		if(!(home = f_env_find_elem(envp, "HOME", "=")))
		{
			ft_putstr_fd("cd: HOME not set", 2);
			return (errno);
		}
		if (chdir(home) == -1)
			return (path_error(home));
		return (0);
	}
	else if (path[0] == '~')
		return (tilda_handle(path, envp));
	if (chdir(path) == -1)
	{
		return (path_error(path));
	}
	return (0);
}
