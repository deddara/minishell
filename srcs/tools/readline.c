/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:18:25 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/01 22:20:44 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

int				f_readline(char **input)
{
	char		*line;
	char		*tmp;
	char		buf[2];
	int			res;

	line = ft_strdup("");
	ft_bzero(buf, 2);
	while(1)
	{
		if ((res = read(0, buf, 1)) == -1)
			return (2);
		if (g_sigquit)
		{
			if (*input)
				free(*input);
			*input = NULL;
			if (line)
			{
				free(line);
				line = ft_strdup("");
			}
			g_sigquit = 0;
		}
		if (!res && !ft_strlen(line))
		{
			ft_putstr_fd("exit\n", 1);
			return (3);
		}
		if (!res)
		{
			ft_putstr_fd("  \b\b", 1);
		}
		if (buf[0] != '\n')
		{
			if (res)
			{
				tmp = line;
				if (!(line = ft_strjoin(line, buf)))
					return (1);
				free(tmp);
			}
		}
		else
		{
			*input = line;
			return (0);
		}
	}
}
