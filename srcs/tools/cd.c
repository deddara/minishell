/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:18:54 by deddara           #+#    #+#             */
/*   Updated: 2020/09/21 21:59:24 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

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


int		f_cd(char *path, char **envp)
{
	char *home;

	if (!path)
	{
		if(!(home = f_env_find_elem(envp, "HOME", "=")))
			return (1);
		if (chdir(home) == -1)
			return (errno);
		return (0);
	}
	if (chdir(path) == -1)
	{
		write (2, "cd: ", 4);
		write (2, path, ft_strlen(path));
		write (2, ": No such file or directory\n", 28);
		return (errno);
	}
	return (0);
}