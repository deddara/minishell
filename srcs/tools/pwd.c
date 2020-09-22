/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:00:58 by deddara           #+#    #+#             */
/*   Updated: 2020/09/22 18:12:17 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void f_pwd(int fd)
{
	char *dir;

	if (!(dir = getcwd(NULL, 0)))
		return ;
	ft_putstr_fd(path, fd);
	ft_putchar_fd('\n', fd);
}
