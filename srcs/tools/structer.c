#include "minishell.h"
#include "structer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

void 				clear_list(t_command *cmd)
{
	t_command *tmp;

	if (cmd)
	{
		while (cmd)
		{
			f_strarr_free(cmd->argv);
			tmp = cmd;
			cmd = cmd->next;
			free (tmp);
		}
		cmd = NULL;
	}
}

t_command	*create_command_lst(void)
{
	t_command *tmp;

	if (!(tmp = malloc(sizeof(t_command) * 1)))
		return (NULL);
	tmp->pipe = 0;
	tmp->redirect = 0;
	tmp->file = 0;
	tmp->flag = 0;
	tmp->flag2 = 0;
	tmp->argv = malloc(sizeof(char) * 2);
	tmp->argv[0] = 0;
	tmp->argv[1] = 0;
	tmp->next = NULL;
	return (tmp);
}

static int			pipe_handler(t_data *data, t_command **cmd_tmp, int i)
{
	if (ft_strncmp(data->inp_arr[i + 1], ">", 1) == 0)
		(*cmd_tmp)->redirect = 1;
	else if (ft_strncmp(data->inp_arr[i + 1], ">>", 2) == 0)
		(*cmd_tmp)->redirect = 2;
	else if (ft_strncmp(data->inp_arr[i + 1], "<", 1) == 0)
		(*cmd_tmp)->redirect = 3;
	(*cmd_tmp)->pipe = 1;
	(*cmd_tmp)->flag2 = 2;
	if (!((*cmd_tmp)->next = create_command_lst()))
		return (1);
	if ((*cmd_tmp)->redirect)
	{
		(*cmd_tmp) = (*cmd_tmp)->next;
		(*cmd_tmp)->file = 1;
		return (0);
	}
	(*cmd_tmp) = (*cmd_tmp)->next;
	return (0);
}

static int			valid_check(t_data *data, int i)
{
	if (data->inp_arr[0][0] == '|')
	{
		write(2, "syntax error near unexpected token `|'\n", 39);
		return (258);
	}
	if (data->inp_arr[i][0] == '|' && !data->inp_arr[i + 1])
	{
		write(2, "undefined behavior: multiple lines\n", 35);
		return (1);
	}
	if (((data->inp_arr[i][0] == '<' || data->inp_arr[i][0] == '>')\
	&& !data->inp_arr[i + 1]) || (data->inp_arr[0][0] == '<' || data->inp_arr[0][0] == '>'))
	{
		write(2, "syntax error near unexpected token `newline'\n", 45);
		return (258);
	}
	return (0);
}

static int			redirect_handler(t_data *data, t_command **cmd_tmp, int i)
{
	if (data->inp_arr[i + 1][0] == '|' && (data->inp_arr[i][0] == '<' || \
			!ft_strncmp(data->inp_arr[i], ">>", 2)))
	{
		write(2, "syntax error near unexpected token `newline'\n", 45);
		return (258);
	}
	if (!ft_strncmp(data->inp_arr[i + 1], "|", 1))
		(*cmd_tmp)->pipe = 1;
	if (ft_strncmp(data->inp_arr[i], ">>", 2) == 0)
		(*cmd_tmp)->redirect = 2;
	else if (ft_strncmp(data->inp_arr[i], ">", 1) == 0)
		(*cmd_tmp)->redirect = 1;
	else if (ft_strncmp(data->inp_arr[i], "<", 1) == 0)
		(*cmd_tmp)->redirect = 3;
	if (!((*cmd_tmp)->next = create_command_lst()))
		return (1);
	*(cmd_tmp) = (*cmd_tmp)->next;
	(*cmd_tmp)->file = 1;
	return (0);
}

static int			struct_handler(t_data *data, t_command **cmd_tmp, int i)
{
	int err_code;

	if (data->inp_arr[i][0] == '|')
	{
		if (pipe_handler(data, cmd_tmp, i))
			return (1);
	}
	else if (data->inp_arr[i][0] == '>' || data->inp_arr[i][0] == '<' || \
		!ft_strncmp(data->inp_arr[i], ">>", 2))
	{
		if ((err_code = redirect_handler(data, cmd_tmp, i)))
			return (err_code);
	}
	else if (!((*cmd_tmp)->argv = \
	f_strarr_add_elem((*cmd_tmp)->argv, data->inp_arr[i])))
	{
		write(2, "malloc error\n", 13);
		return (1);
	}
	return (0);
}

static int			count_symbols(t_data *data, int i, char symb)
{
	int j;

	j = 0;
	while (data->inp_arr[i][0] == symb && data->inp_arr[i])
	{
		i++;
		j++;
	}
	if (symb == '>' && j > 2)
	{
		write(2, "syntax error near unexpected token ", 35);
		j > 3 ? write(2, "`>'\n", 4) : write(2, "`>>'\n", 5);
		return (258);
	}
	if (j > 1)
	{
		write(2, "syntax error near unexpected token `", 36);
		write(2, &symb, 1);
		write(2, "'\n", 2);
		return (258);
	}
	return (0);
}

static int check_for_redir(t_command *cmd)
{
	t_command *cmd_tmp;

	cmd_tmp = cmd;
	while (cmd_tmp)
	{
		if (cmd_tmp->redirect && cmd_tmp->pipe && !cmd_tmp->flag)
		{
			cmd_tmp->flag = 1;
			return (1);
		}
		cmd_tmp = cmd_tmp->next;
	}
	return (0);
}

int					structer(t_data *data, t_command *cmd)
{
	int			i;
	t_command	*cmd_tmp;
	int			err_code;

	cmd_tmp = cmd;
	i = 0;
	if (!(data->inp_arr[0]))
		return (5);
	while (data->inp_arr[i])
	{
		if (check_for_redir(cmd))
		{
			i++;
			continue;
		}
		if ((err_code = valid_check(data, i)))
			return (err_code);
		if (data->inp_arr[i][0] == '|' || data->inp_arr[i][0] == '>' \
		|| data->inp_arr[i][0] == '<')
		{
			if (count_symbols(data, i, data->inp_arr[i][0]))
				return (258);
		}
		if ((err_code = struct_handler(data, &cmd_tmp, i)))
			return (err_code);
		i++;
	}
	return (0);
}
