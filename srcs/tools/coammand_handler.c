#include "minishell.h"
#include "structer.h"
#include <unistd.h>
#include <stdlib.h>

int	command_handler(t_data *data, t_command *cmd)
{
	t_command *tmp;

	tmp = cmd;

	while (tmp)
	{
		if (tmp->file)
		{
			tmp = tmp->next;
			continue;
		}
		if (check_command(data, tmp))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
