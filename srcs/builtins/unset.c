/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 23:17:42 by deddara           #+#    #+#             */
/*   Updated: 2020/09/30 17:34:24 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int			f_unset(t_data *data, char **argv)
{
	int		i;

	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], ' '))
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			return (1);
		}
		if (!(data->envp = f_strarr_rem_elem(data->envp, argv[i], "=")))
			return (f_quit(data, 1, "malloc error\n"));
		i++;
	}
	return (0);
}
