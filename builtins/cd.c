/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:22:35 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/27 15:16:57 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_command *cmd, t_global *global)
{
	char	*path;

	if (cmd->args[2])
	{
		ft_printf("%s %s: too many arguments\n", MINISHELL, cmd->command);
		return (1);
	}
	if (!cmd->args[1])
	{
		path = ft_get_env_var_value(ENV_HOME, global->env);
		if (!path)
		{
			ft_printf("%s %s: HOME not set\n", MINISHELL,
				cmd->command);
			return (1);
		}
		if (chdir(path) == -1)
			return (1);
	}
	else if (chdir(cmd->args[1]) == -1)
	{
		ft_printf("%s %s: No such file or directory\n", MINISHELL,
			cmd->command);
		return (1);
	}
	return (0);
}
