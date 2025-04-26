/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:53:18 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 20:04:20 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_handle_minishell_errors(char *cmd, int status);

void	ft_exit(t_global *global, char *cmd, int status)
{
	if (status)
		ft_handle_minishell_errors(cmd, status);
	if (global)
		free_global(global, true);
	if (global->env)
		ft_lstclear(&global->env, ft_clear_minishell_env);
	exit(status);
}

static void	ft_handle_minishell_errors(char *cmd, int status)
{
	if (ft_strcmp("Fatal", cmd) == 0)
		errmsg_cmd(cmd, "Could not initialize environment",
			strerror(errno), false);
	if (ft_strcmp("command not found", cmd) == 0)
		errmsg_cmd(cmd, NULL, strerror(127), false);
	errmsg_cmd(cmd, NULL, strerror(errno), false);
}
