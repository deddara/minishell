/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strarr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 15:12:26 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/05 14:17:16 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

char			**f_strarr_dup(char **src_arr)
{
	char		**dest_arr;
	int			i;
	int			j;

	if (!src_arr)
		return (NULL);
	i = 0;
	while (src_arr[i])
		i++;
	if (!(dest_arr = (char**)malloc(sizeof(char*) * (i + 1))))
		return (NULL);
	dest_arr[i] = NULL;
	j = -1;
	while (++j < i)
	{
		if (!(dest_arr[j] = ft_strdup(src_arr[j])))
			return (NULL);
	}
	return (dest_arr);
}

char			**f_strarr_add_elem(char **src_arr, char *str)
{
	char		**dest_arr;
	int			i;
	int			j;

	if (!src_arr)
		return (NULL);
	if (!str)
		return (src_arr);
	i = 0;
	while (src_arr[i])
		i++;
	if (!(dest_arr = (char**)malloc(sizeof(char*) * (i + 2))))
		return (NULL);
	dest_arr[i + 1] = NULL;
	j = -1;
	while (++j < i)
		dest_arr[j] = src_arr[j];
	if (!(dest_arr[j] = ft_strdup(str)))
		return (NULL);
	src_arr = f_strarr_free(src_arr);
	free(src_arr);
	return (dest_arr);
}

int				f_strarr_find_elem(char **src_arr, char *str, char *endcmp)
{
	int			i;
	int			len;
	char		*coincidence;

	if (!str)
		return (-1);
	len = ft_strlen(str);
	i = -1;
	while (src_arr[++i])
	{
		if ((coincidence = ft_strnstr(src_arr[i], str, len)) && \
			coincidence == src_arr[i] && (ft_strchr(endcmp, src_arr[i][len]) \
			&& !(!src_arr[i][len] && ft_strlen(endcmp))))
			return (i);
	}
	return (-1);
}

char			**f_strarr_rem_elem(char **src_arr, char *str, char *endcmp)
{
	char		**dest_arr;
	int			i;
	int			j;
	int			k;

	if (!src_arr || !str)
		return (NULL);
	if ((j = f_strarr_find_elem(src_arr, str, endcmp)) < 0)
		return (src_arr);
	i = 0;
	while (src_arr[i])
		i++;
	if (!(dest_arr = (char**)malloc(sizeof(char*) * i--)))
		return (NULL);
	dest_arr[i] = NULL;
	k = -1;
	while (++k < j)
		dest_arr[k] = src_arr[k];
	--k;
	while (++k < i)
		dest_arr[k] = src_arr[k + 1];
	free(src_arr[j]);
	free(src_arr);
	return (dest_arr);
}

void			*f_strarr_free(char **strarr)
{
	int			i;

	i = -1;
	if (strarr)
	{
		while (strarr[++i])
		{
			free(strarr[i]);
			strarr[i] = NULL;
		}
		free(strarr);
	}
	return (strarr = NULL);
}
