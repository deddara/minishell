/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/10/04 13:16:46 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

static void		sorter(char **sorted_env, int j)
{
	char		*tmp;

	tmp = sorted_env[j];
	sorted_env[j] = sorted_env[j + 1];
	sorted_env[j + 1] = tmp;
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

static void		f_print_variable(char **sorted_env, int i, int fd)
{
	int		equal_ind;

	ft_putstr_fd("declare -x ", fd);
	if ((equal_ind = f_coincidence_char_ind(sorted_env[i], '=')) >= 0)
	{
		write(fd, sorted_env[i], equal_ind + 1);
		ft_putchar_fd('\"', fd);
		ft_putstr_fd(sorted_env[i] + equal_ind + 1, fd);
		ft_putchar_fd('\"', fd);
	}
	else
		ft_putstr_fd(sorted_env[i], fd);
	ft_putchar_fd('\n', fd);
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
			f_print_variable(sorted_env, i, fd);
			i++;
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
