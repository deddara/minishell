/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 17:59:28 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/06 12:55:05 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_command		*create_command_lst(void)
{
	t_command	*tmp;

	if (!(tmp = malloc(sizeof(t_command) * 1)))
		return (NULL);
	tmp->pipe = 0;
	tmp->redirect = 0;
	tmp->file = 0;
	tmp->flag = 0;
	tmp->flag2 = 0;
	tmp->argv = NULL;
	tmp->next = NULL;
	return (tmp);
}

t_command		*clear_list(t_command *cmd)
{
	t_command	*tmp;

	if (cmd)
	{
		while (cmd)
		{
			cmd->argv = f_strarr_free(cmd->argv);
			tmp = cmd;
			cmd = cmd->next;
			free(tmp);
		}
		cmd = NULL;
	}
	return (NULL);
}
