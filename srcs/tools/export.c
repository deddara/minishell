/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 00:17:07 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sorter(t_data *data, int j)
{
	char *tmp;

	if (!(tmp = ft_strdup(data->envp[j])))
		return ;
	free(data->envp[j]);
	if (!(data->envp[j] = ft_strdup(data->envp[j + 1])))
		return ;
	free(data->envp[j + 1]);
	if (!(data->envp[j + 1] = ft_strdup(tmp)))
		return ;
	free(tmp);
}

static void	sort_list(t_data *data)
{
	int		i;
	int		j;
	int		n;

	n = 0;
	while (data->envp[n])
		n++;
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strncmp(data->envp[j], data->envp[j + 1], \
			ft_strlen(data->envp[j])) > 0)
				sorter(data, j);
			j++;
		}
		i++;
	}
}

void		f_export(t_data *data, char *str, int fd)
{
	if (!str)
	{
		sort_list(data);
		f_env(fd, data);
		return ;
	}
}
