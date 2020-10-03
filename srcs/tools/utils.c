/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 12:14:28 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 12:33:13 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <signal.h>

int				f_coincidence_char_ind(char *str, char c)
{

	char	*coincidence;

	return ((coincidence = ft_strchr(str, c)) ? coincidence - str : -1);
}

int				f_get_exitcode(int status)
{
	int			sig;

	if (WIFSIGNALED(status))
	{
		if ((sig = WTERMSIG(status)) == SIGQUIT)
		{
			ft_putstr_fd("Quit:\t", 2);
			ft_putnbr_fd(sig, 2);
			ft_putchar_fd('\n', 2);
		}
		return (128 + sig);
	}
	return (WEXITSTATUS(status));
}

char			*f_env_find_elem(char **src_arr, char *str, char *endcmp)
{
	int			i;
	int			len;
	char		*coincidence;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	i = -1;
	while (src_arr[++i])
	{
		if ((coincidence = ft_strnstr(src_arr[i], str, len)) && \
			coincidence == src_arr[i] && (ft_strchr(endcmp, src_arr[i][len]) \
			&& !(!src_arr[i][len] && ft_strlen(endcmp))))
		{
			while (*coincidence != '=')
				coincidence++;
			return (++coincidence);
		}
	}
	return (NULL);
}
