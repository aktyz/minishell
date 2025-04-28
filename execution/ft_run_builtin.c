/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:54:00 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 17:43:37 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_run_builtin(t_command *cmd, t_global *global);
bool	is_parent_builtin(const char *command);

void	ft_run_builtin(t_command *cmd, t_global *global)
{
	if (ft_strncmp(cmd->command, ECHO, ft_strlen(ECHO)) == 0)
		ft_echo(cmd->args);
	if (ft_strncmp(cmd->command, PWD, ft_strlen(PWD)) == 0)
		ft_pwd();
	if (ft_strncmp(cmd->command, ENV, ft_strlen(ENV)) == 0)
		ft_env(global->env);
}

bool	is_parent_builtin(const char *command)
{
	return (ft_strncmp(command, EXIT, ft_strlen(EXIT)) == 0
		|| ft_strncmp(command, CD, ft_strlen(CD)) == 0
		|| ft_strncmp(command, EXPORT, ft_strlen(EXPORT)) == 0
		|| ft_strncmp(command, UNSET, ft_strlen(UNSET)) == 0);
}
