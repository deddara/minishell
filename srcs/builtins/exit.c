/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 17:12:45 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/01 12:48:22 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "structer.h"
#include "libft.h"
#include <stdlib.h>

static void		f_check_arg_for_valid(t_data *data, char **argv, int i)
{
	if ((!i && !(ft_strchr("-+", argv[1][i]) \
		|| ft_isdigit(argv[1][i]))) \
		|| (i && !ft_isdigit(argv[1][i])))
	{
		ft_putstr_fd("exit: ", 1);
		ft_putstr_fd(argv[1], 1);
		ft_putstr_fd(": numeric argument required\n", 1);
		f_quit(data, 0, "");
		exit(255);
	}
}

int				f_exit(t_data *data, char **argv)
{
	int			i;
	int			code;

	i = 0;
	code = 0;
	if (argv[1])
	{
		if (argv[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 1);
			return (1);
		}
		while (argv[1][i])
		{
			f_check_arg_for_valid(data, argv, i);
			i++;
		}
		code = ft_atoi(argv[1]);
	}
	f_quit(data, 0, "");
	ft_putstr_fd("exit\n", 1);
	exit((unsigned char)code);
}
