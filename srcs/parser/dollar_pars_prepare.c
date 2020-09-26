/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_pars_prepare.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:24:49 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/26 16:19:00 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int				f_dollar_pars_prepare(t_data *data, int *i, int *k)
{
	if (f_add_segment(data, *i))
		return (1);
	(*i)++;
	data->last_saved = *i;
	*k = *i;
	while ((ft_isdigit(data->input[*k]) && !(*i - *k)) || (data->input[*i] \
			 && !ft_isdigit(data->input[*k]) \
			 && (data->input[*i] == '_' || ft_isalnum(data->input[*i]))))
		(*i)++;
	*k = *i - *k;
	return (0);
}
