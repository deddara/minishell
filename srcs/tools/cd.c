/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 20:18:54 by deddara           #+#    #+#             */
/*   Updated: 2020/09/21 21:13:44 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
int		f_cd(char *path, char **envp)
{
	if (!path)
	{
		chdir("~/");
		f_pwd();
		return (0);
	}
	if (chdir(path) == -1)
	{
		write (2, "cd: no such file or directory: ", 31);
		write (2, &path, ft_strlen(path));
		printf ("%d", errno);
		return (errno);
	}
	return (0);
}