/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:00:58 by deddara           #+#    #+#             */
/*   Updated: 2020/09/21 20:09:27 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void f_pwd(void)
{
	char *dir;
	if (!(dir = getcwd(NULL, 0)))
		return ;
	write (1, dir, ft_strlen(dir));
	write (1, "\n", 1);
}