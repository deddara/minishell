/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/10/02 12:38:02 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

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

int			check_for_equal(char **words)
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

static char *take_key(char *argv)
{
	int i;
	int count;
	char *tmp;

	count = 0;
	i = 0;
	while (argv[count])
		count++;
	tmp = malloc(count + 1);
	while (argv[i] && argv[i] != '=')
	{
		tmp[i] = argv[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

static int	add_var(t_data *data, char **argv)
{
	int i;
	char	*key;
	char 	*val;
	char 	*val2;

	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], ' ') || !ft_strncmp(argv[i], "=", 1))
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (1);
		}
		key = take_key(argv[i]);
		if (f_strarr_find_elem(data->envp, key, "") != -1 )
		{
			if (!ft_strrchr(argv[i], '='))
				return (0);
			else
				data->envp = f_strarr_rem_elem(data->envp, key, "");
		}
		else if (f_strarr_find_elem(data->envp, key, "=") != -1)
		{
			val = f_env_find_elem(data->envp, key, "=");
			if(!(val2 = ft_strrchr(argv[i], '=')))
				return (0);
			val2++;
			if (!val2)
			{
				data->envp = f_strarr_rem_elem(data->envp, key, "=");
				if (!(data->envp = f_strarr_add_elem(data->envp, argv[i])))
					return (1);
			}
			if (!ft_strncmp(val, val2, ft_strlen(val)) && ft_strlen(val) == ft_strlen(val2))
				return (0);
			data->envp = f_strarr_rem_elem(data->envp, key, "=");
		}
		if (!(data->envp = f_strarr_add_elem(data->envp, argv[i])))
			return (1);
		i++;
	}
	return (0);
}

int			f_export(t_data *data, char **argv, int fd)
{
	char	**sorted_env;
	int		equal_ind;
	int		i;

	i = 0;
	if (!argv[i])
	{
		sorted_env = f_strarr_dup(data->envp);
		sort_list(sorted_env);
		while (sorted_env[i])
		{
			ft_putstr_fd("declare -x ", fd);
			if ((equal_ind = f_coincidence_char_ind(sorted_env[i], '=')) < 0)
				return (1);
			write(fd, sorted_env[i], equal_ind + 1);
			ft_putchar_fd('\"', fd);
			ft_putstr_fd(sorted_env[i++] + equal_ind + 1, fd);
			ft_putchar_fd('\"', fd);
			ft_putchar_fd('\n', fd);
		}
		sorted_env = f_strarr_free(sorted_env);
	}
	else
	{
		if ((add_var(data, argv)))
			return (1);
	}
	return (0);
}
