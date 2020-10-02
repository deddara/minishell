/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:20:46 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 01:56:30 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <unistd.h>

int				f_add_segment(t_data *data, int i)
{
	char		*segment;

	if (!(i - data->last_saved))
		return (0);
	if (!(segment = (char*)malloc(sizeof(char) * i - data->last_saved + 1)))
		return (1);
	ft_strncpy(segment, data->input + data->last_saved, i - data->last_saved);
	segment[i - data->last_saved] = '\0';
	f_join_to_w(data, segment);
	free(segment);
	data->last_saved = i;
	return (0);
}

int				f_join_to_w(t_data *data, char *str)
{
	char		*w_tmp;

	w_tmp = data->w;
	if (!data->w)
	{
		if (!(data->w = ft_strdup(str)))
			return (1);
	}
	else
	{
		if (!(data->w = ft_strjoin(data->w, str)))
			return (1);
	}
	free(w_tmp);
	w_tmp = NULL;
	return (0);
}

int				f_pars_prepare(t_data *data, int *i)
{
	if (f_ind_arr_init(data))
		return (1);
	*i = data->pos;
	data->start = data->pos;
	data->last_saved = data->pos;
	while (data->input[*i] && (data->input[*i] != ';' \
				|| (data->input[*i] == ';' && ((f_quote_status(data) \
				|| f_chk_shield_rev(data, *i))))))
	{
		if ((f_chk_quotes(data, *i)) == 2)
			f_clear_quotes_flags(data);
		(*i)++;
	}
	f_clear_quotes_flags(data);
	data->pars_complete = (!data->input[*i] || ((data->input[*i] == ';') \
							&& !data->input[*i + 1])) ? 1 : 0;
	data->pos = *i;
	*i = data->start;
	free((data->inp_arr) ? data->inp_arr : NULL);
	data->inp_arr = (char**)malloc(sizeof(char*) * 1);
	data->inp_arr[0] = NULL;
	return (0);
}

int				f_word_complete(t_data *data, int *i, int *w_ind)
{
	if (f_chk_unclosed_quotes(data))
		return (1);
	if (f_chk_shield(data, *i) && ft_strchr("><|&", data->input[*i]))
		(*i)++;
	if (f_add_segment(data, *i))
		return (1);
	if (data->w)
	{
		if (!(data->inp_arr = f_strarr_add_elem(data->inp_arr, data->w)))
			return (1);
		(*w_ind)++;
	}
	if (f_process_pars(data, i, w_ind))
		return (1);
	*i += (data->input[*i]) ? 1 : 0;
	while (ft_isspace(data->input[*i]))
		(*i)++;
	data->last_saved = *i;
	if (data->w)
		free(data->w);
	data->w = NULL;
	return (0);
}

int				f_pars_input(t_data *data)
{
	int			i;
	int			res;
	int			w_ind;
	char		*ptr;

	w_ind = 0;
	f_pars_prepare(data, &i);
	while (i < data->pos)
	{
		while (i < data->pos && (((!(ptr = ft_strchr("> <|", data->input[i])) \
				|| (ptr && (res = f_quote_status(data))))) \
				|| (ptr && f_chk_shield(data, i) && !res)))
		{
			if ((res = f_word_chks(data, &i)) == 2)
				continue;
			if (res == 1)
				return (1);
			i++;
		}
		if (f_word_complete(data, &i, &w_ind))
			return (1);
	}
	data->pos += (data->pars_complete) ? 0 : 1;
	return (0);
}
