/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 12:42:55 by awerebea          #+#    #+#             */
/*   Updated: 2020/09/22 18:36:36 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

/*
** fd - file descriptor
** rmndr - remainder of readed line from last iteration of GNL function
** rmndr_start - pointer to start of 'rmndr' string (for 'free')
*/

typedef struct	s_glst
{
	int				fd;
	char			*rmndr;
	char			*rmndr_start;
	struct s_glst	*next;
}				t_glst;

int				get_next_line(int fd, char **line);
t_glst			*f_gnl_new(int fd);
t_glst			*f_search_gnl(int fd, t_glst **g_head);

#endif
