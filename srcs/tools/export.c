/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 22:24:25 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

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

static int	check_for_equal(char **words)
{
	int i;
	int j;
	int flag;

	flag = 0;
	i = 0;
	while (words[i])
	{
		j = 0;
		flag = 0;
		while (words[i][j])
		{
			if (words[i][j] == '=')
				flag = 1;
			j++;
		}
		if (!flag)
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	add_var(t_data *data, char *str)
{
	char	**words;
	int		i;

	i = 0;
	if (!(words = ft_split(str, ' ')))
		return (1);
	if (check_for_equal(words))
		return (1);
	while (words[i])
	{
		if (!(data->envp = f_strarr_add_elem(data->envp, words[i])))
			return (1);
		i++;
	}
	return (0);
}

int			f_export(t_data *data, char *str, int fd)
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
			ft_putchar_fd('\n', fd);
		}
		sorted_env = f_strarr_free(sorted_env);
	}
	else
	{
		if ((add_var(data, str)))
			return (1);
	}
	return (0);
}
