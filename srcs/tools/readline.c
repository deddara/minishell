/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 15:18:25 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/05 22:13:26 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static void sigquit_handle(char **input, char **line)
{
	if (*input)
		free(*input);
	*input = NULL;
	if (*line)
	{
		free(*line);
		*line = ft_strdup("");
	}
	g_sigquit = 0;
}
static int readline_handler(char **line, char buf[2], char **input, int res)
{
	char		*tmp;
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
	line = ft_strdup("");
	ft_bzero(buf, 2);
	while(1)
	{
		if ((res = read(0, buf, 1)) == -1)
			return (2);
		if (g_sigquit)
		sigquit_handle(input, &line);
		if (!res && !ft_strlen(line))
		{
		if (line)
			free(line);
		ft_putstr_fd("exit\n", 1);
			return (3);
		}
		if (!res)
		ft_putstr_fd("  \b\b", 1);
		if ((res = readline_handler(&line, buf, input, res)))
			return (res - 1);
	}
}
