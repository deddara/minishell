/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 12:14:28 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/02 12:24:00 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int				f_coincidence_char_ind(char *str, char c)
{

	char	*coincidence;

	return ((coincidence = ft_strchr(str, c)) ? coincidence - str : -1);
}
