/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:20:46 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/25 14:25:09 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <string.h>

int				f_chk_shield (t_data *data, int i)
{
	return ((i && data->input[i - 1] == '\\' && (data->slash % 2)) ? 1 : 0);
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

int				f_pars_input(t_data *data)
{
	int			i;
	int			k;
	int			quotes;
	char		*c;
	char		proc[3];

	i = data->pos;
	while (data->input[i] && (data->input[i] != ';' || (data->input[i] == ';' \
			&& f_quote_status(data))))
	{
		f_check_quotes(data, i);
		i++;
	}
	f_clear_quotes_flags(data);
	data->pars_complete = (!data->input[i]) ? 1 : 0;
	data->pos = i;
	i = 0;
	k = 0;
	free((data->inp_arr) ? data->inp_arr : NULL);
	data->inp_arr = (char**)malloc(sizeof(char*) * k + 1);
	data->inp_arr[k] = NULL;
	while (i < data->pos)
	{
		while (i < data->pos && (!ft_strchr("> <|", data->input[i]) || (ft_strchr("> <|", data->input[i]) \
				&& f_quote_status(data))))
		{
			if (data->input[i] == '$' && f_quote_status(data) != 1 && !f_chk_shield(data, i))
			{
				char	*name;
				int		j;
				int		index;
				char	*env_val;

				j = 0;
				index = -1;
				env_val = NULL;
				if (f_add_segment(data, i))
					return (1);
				i++;
				data->last_saved = i;
				/* f_check_quotes(data, i); */
				k = 0;
				while ((data->input[i] && !ft_strchr(" \'\"\\", data->input[i])))
				{
					k++;
					i++;
				}
				if(k)
				{
					if (f_check_quotes(data, i - 1) == 2)
						f_clear_quotes_flags(data);
					if (!(name = (char*)malloc(sizeof(char) * k + 1)))
						return (1);
					name[k] = '\0';
					while (k)
						name[j++] = data->input[i - k--];
					if ((index = f_strarr_find_elem(data->envp, name, "=")) != -1)
					{
						if (!(env_val = ft_strdup((ft_strchr(data->envp[index], '=') + 1))))
							return (1);
						if (f_join_to_w(data, env_val))
							return (1);
						free((env_val) ? env_val : NULL);
					}
					data->last_saved = i;
					free((name) ? name : NULL);
				}
				else
				{
					if (f_join_to_w(data, "$"))
						return (1);
					data->last_saved = i;
				}
			}
			if ((quotes = f_check_quotes(data, i)))
			{
				if (f_add_segment(data, i))
					return (1);
				i++;
				data->last_saved = i;
				if (quotes == 2)
					f_clear_quotes_flags(data);
				continue;
			}
			if (data->input[i] == '\\')
			{
				if (f_add_segment(data, i))
					return (1);
				data->last_saved = i;
				data->slash = 1;
				while (data->input[i + data->slash] == '\\')
					data->slash++;
				if (f_quote_status(data) == 1)
				{
					if (f_add_segment(data, i + data->slash))
						return (1);
					i += data->slash - 1;
					data->last_saved = i + 1;
					data->slash = 0;
				}
				else if ((f_quote_status(data) == 2) && (data->input[i + data->slash] != '\"'))
				{
					if (f_add_segment(data, i + data->slash / 2 + 1))
						return (1);
					i += data->slash - 1;
					data->last_saved = i + 1;
				}
				else
				{
					if (f_add_segment(data, i + data->slash / 2))
						return (1);
					i += data->slash - 1;
					data->last_saved = i + 1;
				}
				if (f_quote_status(data) != 1 && f_chk_shield(data, i + 1) && ft_strchr("\" ", data->input[i + 1]))
				{
					if(!(data->errstr = ft_strdup("undefined behavior: empty \
space after escape character '\\'\n")))
						return (1);
					return (1);
				}
			}
			i++;
		}
		if (f_quote_status(data))
		{
			if(!(data->errstr = ft_strdup("undefined behavior: unclosed quote\n")))
				return (1);
			return (1);
		}
		if (f_chk_shield(data, i) && ft_strchr("><|&", data->input[i]))
			i++;
		if (f_add_segment(data, i))
			return (1);
		if (!(data->inp_arr = f_strarr_add_elem(data->inp_arr, data->w)))
			return (1);
		if (data->input[i] && (c = ft_strchr("><|", data->input[i])))
		{
			ft_bzero(proc, 3);
			proc[0] = *c;
			if (*c == '>' && data->input[i + 1] == '>')
			{
				proc[1] = '>';
				i++;
			}
			if (!(data->inp_arr = f_strarr_add_elem(data->inp_arr, proc)))
				return (1);
		}
		i++;
		while (ft_isspace(data->input[i]))
			i++;
		data->last_saved = i;
		free((data->w) ? data->w : NULL);
		data->w = NULL;
	}
	return (0);
}
