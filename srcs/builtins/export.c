/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/10/03 13:42:49 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

static void		sorter(char **sorted_env, int j)
{
	char		*tmp;

	if (!(tmp = ft_strdup(sorted_env[j])))
		return ;
	free(sorted_env[j]);
	if (!(sorted_env[j] = ft_strdup(sorted_env[j + 1])))
		return ;
	free(sorted_env[j + 1]);
	if (!(sorted_env[j + 1] = ft_strdup(tmp)))
		return ;
	free(tmp);
}

static void		sort_list(char **sorted_env)
{
	int			i;
	int			j;
	int			n;

	n = 0;
	while (sorted_env[n])
		n++;
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1], \
			ft_strlen(sorted_env[j])) > 0)
				sorter(sorted_env, j);
			j++;
		}
		i++;
	}
}

int				f_export(t_data *data, char **argv, int fd)
{
	char		**sorted_env;
	int			i;

	i = 0;
	if (!argv[i])
	{
		sorted_env = f_strarr_dup(data->envp);
		sort_list(sorted_env);
		while (sorted_env[i])
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(sorted_env[i++], fd);
			ft_putchar_fd('\n', fd);
		}
		sorted_env = f_strarr_free(sorted_env);
	}
	else
	{
		if ((f_export_add_var(data, argv)))
			return (1);
	}
	return (0);
}
