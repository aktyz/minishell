/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:54:00 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 17:29:28 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_run_builtin(t_command *cmd, t_global *global);
bool	is_parent_builtin(t_command *command);
void	ft_safe_fork(t_global *g, t_command *cmd);
void	ft_is_status_request(t_token *token, t_command *cmd);
void	ft_split_child_parent_run(t_global *g, t_command *cmd);

int	ft_run_builtin(t_command *cmd, t_global *global)
{
	if (ft_strncmp(cmd->command, ECHO, ft_strlen(ECHO)) == 0)
		return (ft_echo(cmd->args));
	if (ft_strncmp(cmd->command, PWD, ft_strlen(PWD)) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd->command, ENV, ft_strlen(ENV)) == 0)
		return (ft_env(global->env));
	return (1);
}

bool	is_parent_builtin(t_command *cmd)
{
	return (ft_strncmp(cmd->command, EXIT, ft_strlen(EXIT)) == 0
		|| ft_strncmp(cmd->command, CD, ft_strlen(CD)) == 0
		|| ft_strncmp(cmd->command, EXPORT, ft_strlen(EXPORT)) == 0
		|| ft_strncmp(cmd->command, UNSET, ft_strlen(UNSET)) == 0
		|| (cmd->is_builtin && cmd->status_request));
}

void	ft_safe_fork(t_global *g, t_command *cmd)
{
	cmd->cmd_pid = fork();
	if (cmd->cmd_pid == -1)
		ft_exit(g, cmd->command, EXIT_FAILURE);
}

void	ft_is_status_request(t_token *token, t_command *cmd)
{
	if (token->status_request)
		cmd->status_request = true;
}

void	ft_split_child_parent_run(t_global *g, t_command *cmd)
{
	if (!is_parent_builtin(cmd))
		ft_safe_fork(g, cmd);
	ft_handle_redirections(cmd);
}
