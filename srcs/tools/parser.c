/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:20:46 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/23 02:55:26 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int				f_check_quotes(t_data *data, int i)
{
	if ((data->input[i] == ''' || data->input[i] == '"') \
			&& data->qt_o < 0 && data->dbl_qt_o < 0)
	{
		if (data->input[i] == ''')
			data->qt_o = i;
		else if (data->input[i] == '"')
			data->dbl_qt_o = i;
		if (!(data->qt_o < 0 && data->dbl_qt_o < 0))
			return (1);
	}
	else if (data->qt_o >= 0 && (data->input[i] == '''))
	{
		data->qt_c = i;
		return (2);
	}
	else if (data->dbl_qt_o >= 0 && (data->input[i] == '"'))
	{
		data->dbl_qt_c = i;
		return (2);
	}
	return (0);
}

int				f_quote_status(t_data *data)
{
	return (((data->qt_o >= 0 && data->qt_c < 0) || \
				(data->dbl_qt_o >= 0 && data->dbl_qt_c < 0)) ? 1 : 0);
}

void			f_clear_flags(t_data *data)
{
	data->quote_open = -1;
	data->quote_close = -1;
	data->dbl_quote_open = -1;
	data->dbl_quote_close = -1;
}

int				f_pars_input(t_data *data)
{
	int			len;
	int			i;
	int			j;
	int			k;

	len = ft_strlen(data->input);
	i = 0;
	while (data->input[i] && (data->input[i] != ';' || (data->input[i] == ';' \
			&& !f_quote_status(data))))
	{
		f_check_quotes(data, i);
		i++;
	}
	f_clear_flags(data);
	j = 0;
	k = 1;
	while (j < i)
	{
		k++;
		while (data->input[j] != ' ' && !f_check_quotes(data, j))
		if
	}
}
