/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deddara <deddara@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 19:34:14 by deddara           #+#    #+#             */
/*   Updated: 2020/09/24 19:34:15 by deddara          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <sys/errno.h>
#include <unistd.h>
int    check_command(t_data *data)
{
    char    *path_p;
    char    **path_data;

    if (!(path_p = f_env_find_elem(data->envp, "PATH", "="))) //if path not found (should return error that we didn't find command.
        return (1);
    if (!(path_data = ft_split(path_p, ':')))
        return (errno);
    int i = 0;
    while (path_data[i])
    {
        ft_putstr_fd(path_data[i], 1);
        write(1, "\n", 1);
        i++;
    }
    return (0);
}