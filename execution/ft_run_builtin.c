/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:54:00 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/11 09:27:52 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_run_builtin(t_command *cmd, t_global *data)
{
	if (ft_strncmp(cmd->command, ECHO, 5) == 0)
		ft_echo(cmd->args);
	if (ft_strncmp(cmd->command, PWD, 4) == 0)
		ft_pwd();
}
