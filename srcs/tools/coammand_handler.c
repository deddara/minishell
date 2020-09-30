#include "minishell.h"
#include "structer.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
//static void to_lowercase(t_command *cmd)
//{
//	char *str;
//
//	if (cmd->argv[0][0])
//	{
//		str = ft_strdup(cmd->argv[0]);
//		if (cmd->argv[0][0] >= 65 && cmd->argv[0][0] <= 90)
//		{
//
//		}
//	}
//}

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
//		to_lowercase(tmp);
		if (check_command(data, tmp))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
