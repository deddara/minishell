/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:45:37 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 00:07:37 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void sort_list(t_data *data, int fd)
{
	int		i;
	int		j;
	int		n;
	char	*tmp;

	n = 0;
	while (data->envp[n])
		n++;
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strcmp(data->envp[j], data->envp[j + 1]) > 0)
			{
				tmp = ft_strdup(data->envp[j]);
				free (data->envp[j]);
				data->envp[j] = ft_strdup(data->envp[j + 1]);
				free (data->envp[j + 1]);
				data->envp[j + 1] = ft_strdup(tmp);
				free (tmp);
			}
			j++;
		}
		i++;
	}
}

void	f_export(t_data *data, char *str, int fd)
{
	if (!str)
	{
		sort_list(data, fd);
		f_env(data, 1);
	}
}