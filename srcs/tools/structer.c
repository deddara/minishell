/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 18:00:29 by deddara           #+#    #+#             */
/*   Updated: 2020/10/04 18:08:48 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

static int		valid_check(t_data *data, int i)
{
	if (data->inp_arr[0][0] == '|' && data->arr[0] == '1')
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		return (258);
	}
	if (!ft_strncmp(data->inp_arr[i], "|", 2) && !data->inp_arr[i + 1] \
			&& data->arr[i] == '1')
	{
		write(2, "undefined behavior: multiple lines\n", 35);
		return (1);
	}
	if (((!ft_strncmp(data->inp_arr[i], "<", 2) \
		|| !ft_strncmp(data->inp_arr[i], ">", 2)) \
		&& !data->inp_arr[i + 1] && data->arr[i] == '1')
		|| ((!ft_strncmp(data->inp_arr[0], "<", 2) \
		|| !ft_strncmp(data->inp_arr[0], ">", 2)) && data->arr[i] == '1'))
	{
		write(2, "syntax error near unexpected token `newline'\n", 45);
		return (258);
	}
	return (0);
}

static int		count_symbols(t_data *data, int i, char symb)
{
	int			j;

	j = 0;
	while (data->inp_arr[i] && data->inp_arr[i][0] == symb)
	{
		i++;
		j++;
	}
	if (symb == '>' && j > 2)
	{
		write(2, "syntax error near unexpected token ", 35);
		j > 3 ? write(2, "`>'\n", 4) : write(2, "`>>'\n", 5);
		return (258);
	}
	if (j > 1)
	{
		write(2, "syntax error near unexpected token `", 36);
		write(2, &symb, 1);
		write(2, "'\n", 2);
		return (258);
	}
	return (0);
}

static int		check_for_redir(t_command *cmd)
{
	t_command	*cmd_tmp;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		if (cmd_tmp->redirect && cmd_tmp->pipe && !cmd_tmp->flag)
		{
			cmd_tmp->flag = 1;
			return (1);
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (0);
}

static int		some_checks(t_data *data, int i)
{
	int			err_code;

	if ((err_code = valid_check(data, i)))
		return (err_code);
	if ((((data->inp_arr[i][0] == '|' || data->inp_arr[i][0] == '<') \
		&& ft_strlen(data->inp_arr[i]) == 1) \
		|| !ft_strncmp(data->inp_arr[i], ">>", 3) \
		|| !ft_strncmp(data->inp_arr[i], ">", 2)) && data->arr[i] == '1')
	{
		if (count_symbols(data, i, data->inp_arr[i][0]))
			return (258);
	}
	return (0);
}

int				structer(t_data *data, t_command *cmd)
{
	int			i;
	t_command	*cmd_tmp;
	int			err_code;

	cmd_tmp = cmd;
	i = 0;
	if (!(data->inp_arr[0]))
		return (5);
	while (data->inp_arr[i])
	{
		if (check_for_redir(cmd))
		{
			i++;
			continue;
		}
		if ((err_code = some_checks(data, i)))
			return (err_code);
		if ((err_code = struct_handler(data, &cmd_tmp, i)))
			return (err_code);
		i++;
	}
	return (0);
}
