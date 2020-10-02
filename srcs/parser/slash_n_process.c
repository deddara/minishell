/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slash_n_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 14:28:31 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 01:53:33 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int		f_slash_pars_assist(t_data *data, int *i)
{
	if (f_quote_status(data) == 1)
	{
		if (f_add_segment(data, *i + data->slash))
			return (1);
		*i += data->slash - 1;
		data->last_saved = *i + 1;
		data->slash = 0;
	}
	else if ((f_quote_status(data) == 2) && (data->slash % 2) \
				&& !ft_strchr("\"$", data->input[*i + data->slash]))
	{
		if (f_add_segment(data, *i + data->slash / 2 + 1))
			return (1);
		*i += data->slash - 1;
		data->last_saved = *i + 1;
	}
	else
	{
		if (f_add_segment(data, *i + data->slash / 2))
			return (1);
		*i += data->slash - 1;
		data->last_saved = *i + 1;
	}
	return (0);
}

int				f_slash_pars(t_data *data, int *i)
{
	if (data->input[*i] == '\\')
	{
		if (f_add_segment(data, *i))
			return (1);
		data->last_saved = *i;
		data->slash = 1;
		while (data->input[*i + data->slash] == '\\')
			data->slash++;
		if (f_slash_pars_assist(data, i))
			return (1);
		if (f_quote_status(data) != 1 && f_chk_shield(data, *i + 1) \
				&& !data->input[*i + 1])
		{
			if (!(data->errstr = ft_strdup("undefined behavior: unclosed \
escape character '\\'\n")))
				return (1);
			return (1);
		}
	}
	return (0);
}

int				f_ind_arr_realloc(t_data *data)
{
	char		*tmp;
	int			i;

	i = -1;
	if (!(tmp = (char*)malloc(sizeof(char) * (data->arr_size + 100))))
		return (1);
	ft_bzero(tmp, data->arr_size + 100);
	while (++i < data->arr_size)
		tmp[i] = data->arr[i];
	free(data->arr);
	data->arr = tmp;
	data->arr_size += 100;
	return (0);
}

int				f_process_pars(t_data *data, int *i, int *w_ind)
{
	char		*c;
	char		proc[3];

	if (data->input[*i] && (c = ft_strchr("><|", data->input[*i])))
	{
		ft_bzero(proc, 3);
		proc[0] = *c;
		if (*c == '>' && data->input[*i + 1] == '>')
		{
			proc[1] = '>';
			(*i)++;
		}
		if (!(data->inp_arr = f_strarr_add_elem(data->inp_arr, proc)))
			return (1);
		if (!(*w_ind < data->arr_size))
		{
			if (f_ind_arr_realloc(data))
				return (1);
		}
		data->arr[*w_ind] = '1';
		(*w_ind)++;
	}
	return (0);
}
