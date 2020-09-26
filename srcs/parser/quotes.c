/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 14:29:15 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/26 14:32:57 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				f_quote_status(t_data *data)
{
	if (data->qt1_o && !data->qt1_c)
		return (1);
	else if (data->qt2_o && !data->qt2_c)
		return (2);
	else
		return (0);
}

void			f_clear_quotes_flags(t_data *data)
{
	data->qt1_o = 0;
	data->qt1_c = 0;
	data->qt2_o = 0;
	data->qt2_c = 0;
}

int				f_quotes_pars(t_data *data, int *i)
{
	int			quotes;

	if ((quotes = f_chk_quotes(data, *i)))
	{
		if (f_add_segment(data, *i))
			return (1);
		(*i)++;
		data->last_saved = *i;
		if (quotes == 2)
			f_clear_quotes_flags(data);
		return (2);
	}
	return (0);
}
