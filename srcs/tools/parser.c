/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:20:46 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/26 12:07:11 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <string.h>

int				f_chk_shield (t_data *data, int i)
{
	return (((i > data->start) && data->input[i - 1] == '\\' && (data->slash % 2)) ? 1 : 0);
}

int				f_check_quotes(t_data *data, int i)
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
	{
		data->qt1_c = 1;
		return (2);
	}
	else if (data->qt2_o && (data->input[i] == '\"') && !f_chk_shield(data, i))
	{
		data->qt2_c = 1;
		return (2);
	}
	return (0);
}

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

int				f_join_to_w(t_data *data, char *str)
{
	char		*w_tmp;

	w_tmp = data->w;
	if (!data->w)
	{
		if(!(data->w = ft_strdup(str)))
			return (1);
	}
	else
	{
		if(!(data->w = ft_strjoin(data->w, str)))
			return (1);
	}
	free(w_tmp);
	w_tmp = NULL;
	return (0);
}

int				f_add_segment(t_data *data, int i)
{
	char		*segment;

	if (!(i - data->last_saved))
		return (0);
	if(!(segment = (char*)malloc(sizeof(char) * i - data->last_saved + 1)))
		return (1);
	ft_strncpy(segment, data->input + data->last_saved, i - data->last_saved);
	segment[i - data->last_saved] = '\0';
	f_join_to_w(data, segment);
	free(segment);
	data->last_saved = i;
	return (0);
}

int				f_dollar_pars(t_data *data, int *i)
{
	char		*name;
	int			k;
	int			j;
	int			index;
	char		*env_val;

	if (data->input[*i] == '$' && f_quote_status(data) != 1 && !f_chk_shield(data, *i))
	{
		j = 0;
		index = -1;
		env_val = NULL;
		if (f_add_segment(data, *i))
			return (1);
		(*i)++;
		data->last_saved = *i;
		/* f_check_quotes(data, *i); */
		k = 0;
		while ((data->input[*i] && !ft_strchr(" \'\"\\$|?", data->input[*i])))
		{
			k++;
			(*i)++;
		}
		if(k)
		{
			if (f_check_quotes(data, *i - 1) == 2)
				f_clear_quotes_flags(data);
			if (!(name = (char*)malloc(sizeof(char) * k + 1)))
				return (1);
			name[k] = '\0';
			while (k)
				name[j++] = data->input[*i - k--];
			if ((index = f_strarr_find_elem(data->envp, name, "=")) != -1)
			{
				if (!(env_val = ft_strdup((ft_strchr(data->envp[index], '=') + 1))))
					return (1);
				if (f_join_to_w(data, env_val))
					return (1);
				free((env_val) ? env_val : NULL);
			}
			data->last_saved = *i;
			free((name) ? name : NULL);
			return (2);
		}
		if (data->input[*i] == '$')
		{
			if(!(data->errstr = ft_strdup("undefined behavior: command '$$' not supported\n")))
				return (1);
			return (1);
		}
		else if (data->input[*i] == '?')
		{
			if (f_join_to_w(data, ft_itoa(data->errcode)))
				return (1);
			(*i)++;
			data->last_saved = *i;
			return (2);
		}
		else if (!data->input[*i] || !ft_strchr("\'\"", data->input[*i]) || f_quote_status(data) == 2)
		{
			if (f_join_to_w(data, "$"))
				return (1);
			data->last_saved = *i;
			return (2);
		}
	}
	return (0);
}

int				f_quotes_pars(t_data *data, int *i)
{
	int			quotes;

	if ((quotes = f_check_quotes(data, *i)))
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

/* int				f_count_slashes( */

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
		if (f_quote_status(data) == 1)
		{
			if (f_add_segment(data, *i + data->slash))
				return (1);
			*i += data->slash - 1;
			data->last_saved = *i + 1;
			data->slash = 0;
		}
		else if ((f_quote_status(data) == 2) && (data->slash % 2) && !ft_strchr("\"$", data->input[*i + data->slash]))
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
		if (f_quote_status(data) != 1 && f_chk_shield(data, *i + 1) && !data->input[*i + 1])
		{
			if(!(data->errstr = ft_strdup("undefined behavior: empty \
space after escape character '\\'\n")))
				return (1);
			return (1);
		}
	}
	return (0);
}

int				f_chk_unclosed_quotes(t_data *data)
{
	int			quotes;

	if ((quotes = f_quote_status(data)))
	{
		if (quotes == 1)
		{
			if(!(data->errstr = ft_strdup("undefined behavior: unclosed quote\n")))
				return (1);
		}
		if (quotes == 2)
		{
			if(!(data->errstr = ft_strdup("undefined behavior: unclosed double quote\n")))
				return (1);
		}
		return (1);
	}
	return (0);
}

int				f_process_pars(t_data *data, int *i)
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
	}
	return (0);
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

int				f_pars_input(t_data *data)
{
	int			i;
	int			res;
	char		*ptr;

	i = data->pos;
	data->start = data->pos;
	data->last_saved = data->pos;
	while (data->input[i] && (data->input[i] != ';' || (data->input[i] == ';' \
			&& ((f_quote_status(data) || f_chk_shield_rev(data, i))))))
	{
		if ((f_check_quotes(data, i)) == 2)
			f_clear_quotes_flags(data);
		i++;
	}
	f_clear_quotes_flags(data);
	data->pars_complete = (!data->input[i] || ((data->input[i] == ';') && !data->input[i + 1])) ? 1 : 0;
	data->pos = i;
	i = data->start;
	free((data->inp_arr) ? data->inp_arr : NULL);
	data->inp_arr = (char**)malloc(sizeof(char*) * 1);
	data->inp_arr[0] = NULL;
	while (i < data->pos)
	{
		while (i < data->pos && (((!(ptr = ft_strchr("> <|", data->input[i])) || (ptr \
				&& (res = f_quote_status(data))))) || (ptr && f_chk_shield(data, i) && !res)))
		{
			if ((res = f_dollar_pars(data, &i)) == 2)
				continue;
			if (res == 1)
				return (1);
			if ((res = f_quotes_pars(data, &i)) == 2)
				continue;
			if (res == 1)
				return (1);
			if (f_slash_pars(data, &i))
				return (1);
			i++;
		}
		if (f_chk_unclosed_quotes(data))
			return (1);
		if (f_chk_shield(data, i) && ft_strchr("><|&", data->input[i]))
			i++;
		if (f_add_segment(data, i))
			return (1);
		if (!(data->inp_arr = f_strarr_add_elem(data->inp_arr, data->w)))
			return (1);
		if (f_process_pars(data, &i))
			return (1);
		i += (data->input[i]) ? 1 : 0;
		while (ft_isspace(data->input[i]))
			i++;
		data->last_saved = i;
		free((data->w) ? data->w : NULL);
		data->w = NULL;
	}
	data->pos += (data->pars_complete) ? 0 : 1;
	return (0);
}
