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
#include <fcntl.h>
#include <string.h>

static void test_print(t_data *data) //тестовая хуйня для создания команды и печати всего массиво (печать можешь закомментить)
{
    data->argv = malloc(2);
    data->argv[0] = malloc(3);
    data->argv[0] = "/bin/ls/";
    data->argv[1] = 0;
//    int i = 0;
//    while (str[i])
//    {
//        ft_putstr_fd(str[i], 1);
//        write(1, "\n", 1);
//        i++;
//    }
}



static int check_existence(char *c_path)
{
    if (open(c_path, O_RDONLY) == -1)
    {
        return (1);
    }
    return (0);
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
            return (1);
        tmp = c_path;
        if (!(c_path = ft_strjoin(c_path, command)))
            return (1);
        free (tmp);
        if (!check_existence(c_path))
            return (0);
        i++;
    }
    ft_putstr_fd(command, 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(strerror(errno), 2);
    ft_putchar_fd('\n', 2);
    return (errno);
}

int    check_command(t_data *data)
{
    char    *path_p;
    char    **path_data;
    int     i;

    test_print(data);
    i = 0;
    if (data->argv[0][0] == '/')
    {
        if (check_existence(data->argv[0]))
        {
            ft_putstr_fd(data->argv[0], 2);
            ft_putstr_fd(": ", 2);
            ft_putstr_fd(strerror(errno), 2);
            ft_putchar_fd('\n', 2);
            return (errno);
        }
        return (0);
    }
    if (!(path_p = f_env_find_elem(data->envp, "PATH", "="))) //if path not found (should return error that we didn't find command.
        return (1);
    if (!(path_data = ft_split(path_p, ':')))
        return (errno);
    return (find_command(path_data, data->argv[0]));
}
