/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 11:43:27 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 18:42:25 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	f_echo(char **argv, int fd, t_command *cmd)
{
	int	i;
	int	n;

	n = 0;
	i = 0;
	if (!argv[i])
	{
		ft_putchar_fd('\n', 1);
		return ;
	}
	if (!ft_strncmp(argv[i], "-n", 3))
	{
		n = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], fd);
		if (argv[i + 1])
			ft_putchar_fd(' ', fd);
		i++;
	}
	if (cmd->redirect)
		ft_putchar_fd(' ', 1);
	else if (!n)
		ft_putchar_fd('\n', 1);
}
