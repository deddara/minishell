/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/03 10:04:06 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 12:00:56 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int		f_check_n_print(t_data *data, int space_separator, \
									int empty_line, int semicolon_count)
{
	if (!empty_line && semicolon_count > 1)
	{
		if (space_separator)
			ft_putstr_fd("minishell: syntax error near \
unexpected token `;'\n", 2);
		else
			ft_putstr_fd("minishell: syntax error near \
unexpected token `;;'\n", 2);
		f_clear_input_data(data);
		return (1);
	}
	if (empty_line)
	{
		if (semicolon_count == 1)
			ft_putstr_fd("minishell: syntax error near \
unexpected token `;'\n", 2);
		if (semicolon_count > 1)
			ft_putstr_fd("minishell: syntax error near \
unexpected token `;;'\n", 2);
		f_clear_input_data(data);
		return (1);
	}
	return (0);
}

static int		f_space_counter(t_data *data, int *i)
{
	int			start;
	int			count;

	start = *i;
	count = 0;
	while (ft_isspace(data->input[*i]))
	{
		(*i)++;
	}
	return ((count = *i - start) ? count : 0);
}

static void		f_inner_loop(t_data *data, int *i, \
								int *semicolon_count, int *empty_line)
{
	int			start;

	start = *i;
	while (data->input[*i] && (data->input[*i] != ';' \
			|| (data->input[*i] == ';' && ((f_quote_status(data) \
			|| f_chk_shield_rev(data, *i))))))
	{
		if ((f_chk_quotes(data, *i)) == 2)
			f_clear_quotes_flags(data);
		(*i)++;
	}
	if (*i - start)
	{
		*semicolon_count = 0;
		*empty_line = 0;
	}
}

int				f_input_validator(t_data *data)
{
	int			i;
	int			semicolon_count;
	int			space_separator;
	int			empty_line;

	i = 0;
	semicolon_count = 0;
	empty_line = 1;
	while (data->input[i])
	{
		space_separator = f_space_counter(data, &i);
		f_inner_loop(data, &i, &semicolon_count, &empty_line);
		while (data->input[i] == ';')
		{
			semicolon_count++;
			i++;
		}
		f_clear_quotes_flags(data);
		if (f_check_n_print(data, space_separator, empty_line, semicolon_count))
			return (1);
	}
	return (0);
}
