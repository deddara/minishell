/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:20:46 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/23 20:17:25 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <string.h>

int				f_check_quotes(t_data *data, int i)
{
	if (!(data->input[i] == '\'' || data->input[i] == '\"'))
		return (0);
	if (data->qt_o < 0 && data->dbl_qt_o < 0)
	{
		if (data->input[i] == '\'')
			data->qt_o = i;
		else if (data->input[i] == '\"')
			data->dbl_qt_o = i;
		if (!(data->qt_o < 0 && data->dbl_qt_o < 0))
			return (1);
	}
	else if (data->qt_o >= 0 && (data->input[i] == '\''))
	{
		data->qt_c = i;
		return (2);
	}
	else if (data->dbl_qt_o >= 0 && (data->input[i] == '\"'))
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

void			f_clear_quotes_flags(t_data *data)
{
	data->qt_o = -1;
	data->qt_c = -1;
	data->dbl_qt_o = -1;
	data->dbl_qt_c = -1;
}

int				f_add_segment(t_data *data, int i)
{
	char		*segment;
	char		*w_tmp;

	if(!(segment = (char*)malloc(sizeof(char) * i - data->last_saved + 1)))
		return (1);
	strncpy(segment, data->input + data->last_saved, i - data->last_saved);
	segment[i - data->last_saved] = '\0';
	w_tmp = data->w;
	if(!(data->w = ft_strjoin(data->w, segment)))
		return (1);
	free(w_tmp);
	free(segment);
	data->last_saved = i + 1;
	return (0);
}

int				f_pars_input(t_data *data)
{
	int			len;
	int			i;
	int			k;

	len = ft_strlen(data->input);
	i = data->pos;
	while (data->input[i] && (data->input[i] != ';' || (data->input[i] == ';' \
			&& !f_quote_status(data))))
	{
		f_check_quotes(data, i);
		i++;
	}
	f_clear_quotes_flags(data);
	data->pars_complete = (!data->input[i]) ? 1 : 0;
	data->pos = (data->input[i]) ? i + 1 : i;
	i = 0;
	k = 0;
	free((data->inp_arr) ? data->inp_arr : NULL);
	data->inp_arr = (char**)malloc(sizeof(char*) * k + 1);
	data->inp_arr[k] = NULL;
	while (i < data->pos)
	{
		while (i < data->pos && (!ft_strchr("> <|", data->input[i]) || (ft_strchr("> <|", data->input[i]) \
				&& !f_quote_status(data))))
		{
			if(f_check_quotes(data, i) == 1)
			{
				if (f_add_segment(data, i))
					return (f_exit(data, 1, "malloc error\n"));
			}
			if(f_check_quotes(data, i) == 2)
			{
				if (f_add_segment(data, i))
					return (f_exit(data, 1, "malloc error\n"));
				f_clear_quotes_flags(data);
			}
			i++;
		}
		if (f_add_segment(data, i))
			return (f_exit(data, 1, "malloc error\n"));
		if (!(data->inp_arr = f_strarr_add_elem(data->inp_arr, data->w)))
			return (f_exit(data, 1, "malloc error\n"));
		free((data->w) ? data->w : NULL);
	}
	return (0);
}
