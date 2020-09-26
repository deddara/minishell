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

void	f_echo(char *str, int fd, int n)
{
	ft_putstr_fd(str, fd);
	if (!n)
	ft_putchar_fd('\n', 1);
}
