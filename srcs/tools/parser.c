/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 21:20:46 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/23 22:12:50 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

static char *own_strcpy(t_data *data, int first_pos, int last_pos, int pos)
{
	int i;
	int j;
	char *new_arr;

	i = 0;
	j = 0;
	new_arr = malloc(sizeof(char) * ft_strlen(data->input) - 2);
	while (i < pos)
	{
		if (i == first_pos || i == last_pos)
		{
			i++;
			continue;
		}
		new_arr[j] = data->input[i];
		j++;
		i++;
	}
	new_arr[j] = '\0';
	return (new_arr);
}

int				f_pars_input(t_data *data)
{
	int			len;
	int			i;

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
	/* if (data->input[i] && (data->input[i] != ' ' || (data->input[i] == ' ' \ */
	/*         && !f_quote_status(data))))                                      */
	/* {                                                                        */
	/*     if ((f_check_quotes(data, i)) == 2);                                 */
	/*     data->pos = i + 1;                                                   */
	/* }                                                                        */
	i = 0;
	free((data->inp_arr) ? data->inp_arr : NULL);
	data->inp_arr = (char**)malloc(sizeof(char*) * 1);
	data->inp_arr[0] = NULL;

	int flag = 0;
	int first_pos;
	int last_pos;
	char *tmp;
	char *tmp2;
	char sign;
	int j = 0;
	char *delim;
	delim = NULL;
	while (i < data->pos)
	{
		if ((ft_strrchr("<> |", data->input[i]) != NULL || i + 1 == data->pos) && flag == 0)
		{
			if (i + 1 == data->pos)
				i++;
			tmp2 = malloc(i - j + 1);
			delim = malloc(2);
			delim[0] = data->input[i];
			delim[1] = '\0';
			tmp2 = ft_strncpy(tmp2, &data->input[j], i - j);
			if(!(data->inp_arr = f_strarr_add_elem(data->inp_arr, tmp2)))
				write(1, "error", 5);
			if(!(data->inp_arr = f_strarr_add_elem(data->inp_arr, delim)))
				write(1, "error", 5);
			i++;
			j = i;
			free(tmp2);
			free (delim);
			continue;
		}
		if ((data->input[i] == '\'' || data->input[i] == '\"') && flag == 0)
		{	
			first_pos = i;
			sign = data->input[i];
			flag = 1;
			i++;
			continue;
		}
		if (data->input[i] == sign && flag == 1)
		{
			last_pos = i;
			tmp = data->input;
			data->input = own_strcpy(data, first_pos, last_pos, data->pos);
			free (tmp);
			flag = 0;
			i-=1;
			continue;
		}
		i++;
	}
	data->pos += (!data->pars_complete) ? 1 : 0;
	return (0);
}
