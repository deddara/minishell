/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 17:12:45 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/30 18:54:11 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "structer.h"
#include "libft.h"
#include <stdlib.h>

int				f_exit(t_data *data, t_command *cmd)
{
	int			i;
	int			code;

	i = 0;
	code = 0;
	if (cmd->argv[1])
	{
		if (cmd->argv[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 1);
			return (1);
		}
		while (cmd->argv[1][i])
		{
			if ((!i && !(ft_strchr("-+", cmd->argv[1][i]) \
				|| ft_isdigit(cmd->argv[1][i]))) \
				|| (!ft_isdigit(cmd->argv[1][i])))
			{
				ft_putstr_fd("exit: ", 1);
				ft_putstr_fd(cmd->argv[1], 1);
				ft_putstr_fd(": numeric argument required\n", 1);
				f_quit(data, 0, "");
				exit(255);
			}
			i++;
		}
		code = ft_atoi(data->inp_arr[1]);
	}
	f_quit(data, 0, "");
	exit(code);
}
