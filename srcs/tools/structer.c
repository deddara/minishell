
#include "minishell.h"
#include "structer.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
static t_command	*create_command_lst(void)
{
	t_command *tmp;

	if (!(tmp = malloc(sizeof(t_command) * 1)))
		return (NULL);
	tmp->pipe = 0;
	tmp->redirect = 0;
	tmp->argv = NULL;
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
		if (data->inp_arr[i][0] == '|')
		{
			cmd_tmp->pipe = 1;
			cmd_tmp->next = create_command_lst();
			cmd_tmp = cmd_tmp->next;
		}
		i++;
	}
	return (0);
}