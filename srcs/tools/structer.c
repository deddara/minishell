
#include "minishell.h"
#include "structer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
static t_command	*create_command_lst(void)
{
	t_command *tmp;

	if (!(tmp = malloc(sizeof(t_command) * 1)))
		return (NULL);
	tmp->pipe = 0;
	tmp->redirect = 0;
	tmp->argv = malloc(sizeof(char) * 2);
	tmp->argv[0] = 0;
	tmp->argv[1] = 0;
	tmp->next = NULL;
	return (tmp);
}


int		structer(t_data *data)
{
	int			i;
	t_command	*cmd;
	t_command	*cmd_tmp;

	if (!(cmd = create_command_lst()))
		return (1);
	cmd_tmp = cmd;
	i = 0;
	while (data->inp_arr[i])
	{
		if (data->inp_arr[0][0] == '|')
		{
			write (2, "syntax error near unexpected token `|'\n", 39);
			return (258);
		}
		if (data->inp_arr[i][0] == '|' && !data->inp_arr[i + 1])
		{
			write(2, "undefined behavior: multiple lines\n", 35);
			return (1);
		}
		if ((data->inp_arr[i][0] == '<' || data->inp_arr[i][0] == '>') \
		&& !data->inp_arr[i + 1])
		{
			write(2, "syntax error near unexpected token `newline'\n", 45);
			return (1);
		}
		if (data->inp_arr[i][0] == '|')
		{
			if (ft_strncmp(data->inp_arr[i + 1], ">", 1) == 0)
				cmd_tmp->redirect = 1;
			else if (ft_strncmp(data->inp_arr[i + 1], ">>", 2) == 0)
				cmd_tmp->redirect = 2;
			else if (ft_strncmp(data->inp_arr[i + 1], "<", 1) == 0)
				cmd_tmp->redirect = 3;
			cmd_tmp->pipe = 1;
			cmd_tmp->next = create_command_lst();
			cmd_tmp = cmd_tmp->next;
			i++;
			continue;
		}
		else if (data->inp_arr[i][0] == '>' || data->inp_arr[i][0] == '<' || \
		!ft_strncmp(data->inp_arr[i], ">>", 2))
		{
			if (data->inp_arr[i + 1][0] == '|' && (data->inp_arr[i][0] == '<' || \
			!ft_strncmp(data->inp_arr[i], ">>", 2)))
			{
				write(2, "syntax error near unexpected token `newline'\n", 45);
				return (258);
			}
			if (ft_strncmp(data->inp_arr[i], ">", 1) == 0)
				cmd_tmp->redirect = 1;
			else if (ft_strncmp(data->inp_arr[i], ">>", 2) == 0)
				cmd_tmp->redirect = 2;
			else if (ft_strncmp(data->inp_arr[i], "<", 1) == 0)
				cmd_tmp->redirect = 3;
			cmd_tmp->next = create_command_lst();
			cmd_tmp = cmd_tmp->next;
			i++;
			continue;
		}
		if (!(cmd_tmp->argv = f_strarr_add_elem(cmd_tmp->argv, data->inp_arr[i])))
		{
			write(2, "malloc error\n", 13);
			return (1);
		}
		i++;
	}
//	while (cmd)
//	{
//		int j = 0;
//		while (cmd->argv[j]) {
//			ft_putstr_fd(cmd->argv[j], 1);
//			ft_putstr_fd("\n", 1);
//			j++;
//		}
//		cmd = cmd->next;
//	}
	return (0);
}