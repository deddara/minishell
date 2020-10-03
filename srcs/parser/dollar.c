/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 14:27:49 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/03 12:03:26 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int		f_dollar_pars_name_exist(t_data *data, int i, int k)
{
	int			j;
	int			index;
	char		*name;
	char		*env_val;

	j = 0;
	index = -1;
	env_val = NULL;
	if (f_chk_quotes(data, i - 1) == 2)
		f_clear_quotes_flags(data);
	if (!(name = (char*)malloc(sizeof(char) * k + 1)))
		return (1);
	name[k] = '\0';
	while (k)
		name[j++] = data->input[i - k--];
	if ((index = f_strarr_find_elem(data->envp, name, "=")) != -1)
	{
		if (!(env_val = ft_strdup((ft_strchr(data->envp[index], '=') + 1))) \
			|| f_join_to_w(data, env_val))
			return (1);
		free(env_val);
	}
	data->last_saved = i;
	free(name);
	return (2);
}

static int		f_double_dollar_err(t_data *data)
{
	if (!(data->errstr = ft_strdup("undefined behavior: command '$$' not \
supported\n")))
		return (1);
	return (1);
}

static int		f_dollar_question_or_zero(t_data *data, int *i)
{
	char		*num;

	if (data->input[*i] == '?')
	{
		if (!(num = ft_itoa(data->errcode)) || f_join_to_w(data, num))
			return (1);
		(*i)++;
		data->last_saved = *i;
		free(num);
	}
	else
	{
		if (f_join_to_w(data, data->minishell_argv[0]))
			return (1);
		(*i)++;
		data->last_saved = *i;
	}
	return (2);
}

static int		f_join_dollar_sym_to_w(t_data *data, int *i)
{
	if (f_join_to_w(data, "$"))
		return (1);
	data->last_saved = *i;
	return (2);
}

int				f_dollar_pars(t_data *data, int *i)
{
	int			k;

	if (data->input[*i] == '$' && f_quote_status(data) != 1 \
			&& !f_chk_shield(data, *i))
	{
		if (f_dollar_pars_prepare(data, i, &k))
			return (1);
		if (k)
			return (f_dollar_pars_name_exist(data, *i, k));
		if (data->input[*i] == '$')
			return (f_double_dollar_err(data));
		else if (ft_strchr("0?", data->input[*i]))
			return (f_dollar_question_or_zero(data, i));
		else if (!data->input[*i] || !ft_strchr("\'\"", data->input[*i]) \
					|| f_quote_status(data) == 2)
			return (f_join_dollar_sym_to_w(data, i));
	}
	return (0);
}
