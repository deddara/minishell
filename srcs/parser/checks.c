/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 14:24:25 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/04 15:01:28 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int				f_chk_shield(t_data *data, int i)
{
	return (((i > data->start) && data->input[i - 1] == '\\' \
				&& (data->slash % 2)) ? 1 : 0);
}

int				f_chk_shield_rev(t_data *data, int i)
{
	int			k;

	k = 0;
	while ((i - 1 >= data->start) && data->input[i-- - 1] == '\\')
		k++;
	if (k)
		return ((k % 2) ? 1 : 0);
	return (0);
}

int				f_chk_quotes(t_data *data, int i)
{
	if (!(data->input[i] == '\'' || data->input[i] == '\"'))
		return (0);
	if (!data->qt1_o && !data->qt2_o)
	{
		if (data->input[i] == '\'' && !f_chk_shield(data, i))
			data->qt1_o = 1;
		else if (data->input[i] == '\"' && !f_chk_shield(data, i))
			data->qt2_o = 1;
		if (data->qt1_o || data->qt2_o)
			return (1);
	}
	else if (data->qt1_o && (data->input[i] == '\'') && !f_chk_shield(data, i))
		data->qt1_c = 1;
	else if (data->qt2_o && (data->input[i] == '\"') && !f_chk_shield(data, i))
		data->qt2_c = 1;
	return ((data->qt1_c || data->qt2_c) ? 2 : 0);
}

int				f_word_chks(t_data *data, int *i)
{
	int			res;

	if ((res = f_dollar_pars(data, i)) == 2)
		return (2);
	if (res == 1)
		return (1);
	if ((res = f_quotes_pars(data, i)) == 2)
		return (2);
	if (res == 1)
		return (1);
	if (f_slash_pars(data, i))
		return (1);
	return (0);
}

int				f_chk_unclosed_quotes(t_data *data)
{
	int			quotes;

	if ((quotes = f_quote_status(data)))
	{
		ft_putstr_fd("undefined behavior: unclosed ", 2);
		ft_putstr_fd((quotes == 1) ? "quote\n" : "double quote\n", 2);
	}
	return ((quotes) ? 1 : 0);
}
