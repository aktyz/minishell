/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:22:35 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 20:38:29 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_command *cmd, t_global *global)
{
	char	*path;

	if (!cmd->args[1])
	{
		path = ft_get_env_var_value(ENV_HOME, global->env);
		if (!path)
		{
			ft_printf("cd: HOME not set\n");
			return (1);
		}
	}
	if (chdir(cmd->args[1]) == -1)
		return (1);
	return (0);
}
