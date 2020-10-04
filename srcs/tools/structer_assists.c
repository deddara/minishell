/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structer_assists.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 18:06:30 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/04 18:11:22 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

static int		pipe_handler(t_data *data, t_command **cmd_tmp, int i)
{
	if (ft_strncmp(data->inp_arr[i + 1], ">", 1) == 0)
		(*cmd_tmp)->redirect = 1;
	else if (ft_strncmp(data->inp_arr[i + 1], ">>", 2) == 0)
		(*cmd_tmp)->redirect = 2;
	else if (ft_strncmp(data->inp_arr[i + 1], "<", 1) == 0)
		(*cmd_tmp)->redirect = 3;
	(*cmd_tmp)->pipe = 1;
	(*cmd_tmp)->flag2 = 2;
	if (!((*cmd_tmp)->next = create_command_lst()))
		return (1);
	if ((*cmd_tmp)->redirect)
	{
		(*cmd_tmp) = (*cmd_tmp)->next;
		(*cmd_tmp)->file = 1;
		return (0);
	}
	(*cmd_tmp) = (*cmd_tmp)->next;
	return (0);
}

static int		redirect_handler(t_data *data, t_command **cmd_tmp, int i)
{
	if (data->inp_arr[i + 1][0] == '|' && (data->inp_arr[i][0] == '<' || \
			!ft_strncmp(data->inp_arr[i], ">>", 2)))
	{
		write(2, "syntax error near unexpected token `newline'\n", 45);
		return (258);
	}
	if (!ft_strncmp(data->inp_arr[i + 1], "|", 1))
		(*cmd_tmp)->pipe = 1;
	if (ft_strncmp(data->inp_arr[i], ">>", 2) == 0)
		(*cmd_tmp)->redirect = 2;
	else if (ft_strncmp(data->inp_arr[i], ">", 1) == 0)
		(*cmd_tmp)->redirect = 1;
	else if (ft_strncmp(data->inp_arr[i], "<", 1) == 0)
		(*cmd_tmp)->redirect = 3;
	if (!((*cmd_tmp)->next = create_command_lst()))
		return (1);
	*(cmd_tmp) = (*cmd_tmp)->next;
	(*cmd_tmp)->file = 1;
	return (0);
}

int				struct_handler(t_data *data, t_command **cmd_tmp, int i)
{
	int			err_code;

	if (!ft_strncmp(data->inp_arr[i], "|", 2) && data->arr[i] == '1')
	{
		if (pipe_handler(data, cmd_tmp, i))
			return (1);
	}
	else if ((!ft_strncmp(data->inp_arr[i], "<", 2) \
		|| !ft_strncmp(data->inp_arr[i], ">", 2) \
		|| !ft_strncmp(data->inp_arr[i], ">>", 3)) && data->arr[i] == '1')
	{
		if ((err_code = redirect_handler(data, cmd_tmp, i)))
			return (err_code);
	}
	else if (!((*cmd_tmp)->argv = \
	f_strarr_add_elem((*cmd_tmp)->argv, data->inp_arr[i])))
	{
		write(2, "malloc error\n", 13);
		return (1);
	}
	return (0);
}
