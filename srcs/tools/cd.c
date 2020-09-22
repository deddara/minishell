/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:18:54 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 18:14:00 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <errno.h>
#include <string.h>

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
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		return (errno);
	}
	return (0);
}
