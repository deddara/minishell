/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 00:30:15 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sorter(char **sorted_env, int j)
{
	char *tmp;

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

static void	sort_list(char **sorted_env)
{
	int		i;
	int		j;
	int		n;

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

void		two_d_arr_free(char **str)
{
	int i;

	i = 0;
	if (str)
	{
		while (str[i])
		{
			free(str[i]);
			i++;
		}
		free(str);
	}
}

static void	add_var(t_data *data, char *str)
{
	char	**words;
	int		i;

	i = 0;
	if (!(words = ft_split(str, ' ')))
		return ;
	while (words[i])
	{
		if (!(data->envp = f_strarr_add_elem(data->envp, words[i])))
			return ;
		i++;
	}
}

void		f_export(t_data *data, char *str, int fd)
{
	char	**sorted_env;
	int		i;

	i = 0;
	if (!str)
	{
		sorted_env = f_strarr_dup(data->envp);
		sort_list(sorted_env);
		while (sorted_env[i])
		{
			ft_putstr_fd(sorted_env[i++], fd);
			write(fd, "\n", 1);
		}
		two_d_arr_free(sorted_env);
		return ;
	}
	else
		add_var(data, str);
	f_env(1, data);
}
