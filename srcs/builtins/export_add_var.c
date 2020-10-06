/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 18:07:25 by awerebea          #+#    #+#             */
/*   Updated: 2020/10/06 11:02:16 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

static int		f_export_print_err(char **argv, int i)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(argv[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static char		*f_export_take_key(char *argv)
{
	int			i;
	int			count;
	char		*tmp;

	count = 0;
	i = 0;
	while (argv[count])
		count++;
	if (!(tmp = (char*)malloc(sizeof(char) * count + 1)))
		return (NULL);
	while (argv[i] && argv[i] != '=')
	{
		tmp[i] = argv[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

static int		f_var_rem_or_repl(t_data *data, char **argv, \
										int i, char *key)
{
	char		*val;
	char		*val2;

	val = f_env_find_elem(data->envp, key, "=");
	if (!(val2 = ft_strrchr(argv[i], '=')))
		return (1);
	val2++;
	if (!val2)
	{
		data->envp = f_strarr_rem_elem(data->envp, key, "=");
		if (!(data->envp = f_strarr_add_elem(data->envp, argv[i])))
			return (2);
	}
	if (!ft_strncmp(val, val2, ft_strlen(val)) \
			&& ft_strlen(val) == ft_strlen(val2))
		return (1);
	data->envp = f_strarr_rem_elem(data->envp, key, "=");
	return (0);
}

static int		f_check_var_exist(t_data *data, char **argv, int i)
{
	char		*key;
	int			errcode;

	if (!(key = f_export_take_key(argv[i])))
		return (1);
	if (f_strarr_find_elem(data->envp, key, "") != -1)
	{
		if (!ft_strrchr(argv[i], '='))
			return (1);
		else
			data->envp = f_strarr_rem_elem(data->envp, key, "");
	}
	else if (f_strarr_find_elem(data->envp, key, "=") != -1)
	{
		if ((errcode = f_var_rem_or_repl(data, argv, i, key)))
			return (errcode);
	}
	free(key);
	return (0);
}

int				f_export_add_var(t_data *data, char **argv)
{
	int			i;
	int			errcode;

	i = -1;
	while (argv[++i])
	{
		if (ft_strchr(argv[i], ' ') || !ft_strncmp(argv[i], "=", 1) \
				|| !ft_isalpha(argv[i][0]))
			return (f_export_print_err(argv, i));
		if ((errcode = f_check_var_exist(data, argv, i)))
			return (errcode - 1);
		if (!(data->envp = f_strarr_add_elem(data->envp, argv[i])))
			return (1);
	}
	return (0);
}
