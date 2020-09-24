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

static void test_print(char **str)
{
    int i = 0;
    while (str[i])
    {
        ft_putstr_fd(str[i], 1);
        write(1, "\n", 1);
        i++;
    }
}

static int check_existence(char *c_path)
{
    int id;

    if (open(c_path) == -1)
        return (0);
    return (1);
}

static int find_command(char **path_data, char *command)
{
    int     i;
    char    *tmp;
    char    *c_path;

    c_path = NULL;
    i = 0;
    while (path_data[i])
    {
        if (!(c_path = ft_strjoin(path_data[i], "/")))
            return (0);
        tmp = c_path;
        if (!(c_path = ft_strjoin(c_path, command)))
            return (0);
        free (tmp);
        return (check_existence(c_path));
    }
    return (0);
}

int    check_command(t_data *data)
{
    char    *path_p;
    char    **path_data;
    int     i;

    i = 0;
    if (data->argv[0][0] == '/')
    {
        return (check_existence(data->argv[0]));
    }
    if (!(path_p = f_env_find_elem(data->envp, "PATH", "="))) //if path not found (should return error that we didn't find command.
        return (1);
    if (!(path_data = ft_split(path_p, ':')))
        return (errno);
    return (find_command(path_data, data->argv[0]));
}
