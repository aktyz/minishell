/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:22:35 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/11 23:00:35 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_command *cmd)
{
	char	*path;

	if (!cmd->args[1])
	{
		path = getenv(ENV_HOME);
		if (!path)
		{
			ft_printf("cd: HOME not set\n");
			return ;
		}
	}
	if (chdir(cmd->args[1]) == -1)
		ft_printf("cd: %s: No such file or directory\n", cmd->args[1]);
}
