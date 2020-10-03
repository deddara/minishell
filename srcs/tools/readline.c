/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:18:25 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 15:07:24 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static void		f_sigquit_case(char **input, char *line)
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

static int		f_other_cases(char **input, char **line, char *buf, int res)
{
	char		*tmp;

	if (!res && !ft_strlen(*line))
	{
		ft_putstr_fd("exit\n", 1);
		return (4);
	}
	if (!res)
		ft_putstr_fd("  \b\b", 1);
	if (buf[0] != '\n')
	{
		if (res)
		{
			tmp = *line;
			if (!(*line = ft_strjoin(*line, buf)))
				return (2);
			free(tmp);
		}
	}
	else
	{
		*input = *line;
		return (1);
	}
	return (0);
}

int				f_readline(char **input)
{
	char		*line;
	char		buf[2];
	int			res;
	int			result;

	line = ft_strdup("");
	ft_bzero(buf, 2);
	while (1)
	{
		if ((res = read(0, buf, 1)) == -1)
			return (2);
		if (g_sigquit)
			f_sigquit_case(input, line);
		if ((result = f_other_cases(input, &line, buf, res)))
			return (result - 1);
	}
}
