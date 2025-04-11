/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:22:35 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/11 09:24:52 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cd(t_command *cmd)
{
	if (!cmd->args[1])
	{
		ft_printf("cd: missing argument\n");
	}
	if (chdir(cmd->args[1]) == -1)
		ft_printf("cd: %s: No such file or directory\n", cmd->args[1]);
}
