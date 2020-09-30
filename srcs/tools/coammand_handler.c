#include "minishell.h"
#include "structer.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

static void check_for_existence(t_command *cmd, char *str, int flag, t_data *data)
{
	if (!ft_strncmp(str, "cd", 3) && flag)
		data->cd = 1;
	else if (!ft_strncmp(str, "env", 4))
	{
		free(cmd->argv[0]);
		cmd->argv[0] = ft_strdup(str);
	}
	else if (!ft_strncmp(str, "echo", 5))
	{
		free(cmd->argv[0]);
		cmd->argv[0] = ft_strdup(str);
	}
	else if (!ft_strncmp(str, "pwd", 4))
	{
		free(cmd->argv[0]);
		cmd->argv[0] = ft_strdup(str);
	}
	free(str);
}

static void to_lowercase(t_command *cmd, t_data *data)
{
	char *str;
	int i;
	int flag;

	flag = 0;
	i = 0;
	if (cmd->argv[0])
	{
		str = ft_strdup(cmd->argv[0]);
		while (str[i])
		{
			if (str[i] >= 65 && str[i] <= 90)
			{
				str[i] = (char)ft_tolower(str[i]);
				flag = 1;
			}
			i++;
		}
		check_for_existence(cmd, str, flag, data);
	}
}

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
		to_lowercase(tmp, data);
		if (check_command(data, tmp))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
