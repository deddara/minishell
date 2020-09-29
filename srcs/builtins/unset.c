/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:17:42 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 22:38:05 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int		f_unset(t_data *data, char **argv)
{
	int		i;

	i = 0;
	while (argv[i])
	{
		if (!(data->envp = f_strarr_rem_elem(data->envp, argv[i], "=")))
			return (f_exit(data, 1, "malloc error\n"));
		i++;
	}
	return (0);
}
