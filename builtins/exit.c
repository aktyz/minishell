/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 18:53:18 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 15:53:13 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_exit(t_global *global, char *cmd, int status);
void		ft_mini_exit_wrapper(t_command *cmd, t_global *g);
static void	ft_handle_minishell_errors(char *cmd);
static int	ft_is_numeric_arg(const char *c);
static void	handle_exit_err(t_command *cmd, t_global *g, int code);

void	ft_exit(t_global *global, char *cmd, int status)
{
	if (status && cmd && ft_strcmp(cmd, EXIT))
		ft_handle_minishell_errors(cmd);
	if (global)
		free_global(global, true);
	if (global->env)
		ft_lstclear(&global->env, ft_clear_minishell_env);
	if (global->is_global == true)
		ft_printf("exit\n");
	exit(status);
}

static void	ft_handle_minishell_errors(char *cmd)
{
	if (ft_strcmp("Fatal", cmd) == 0)
		errmsg_cmd(cmd, "Could not initialize environment",
			strerror(errno), false);
	if (ft_strcmp("command not found", cmd) == 0)
		errmsg_cmd(cmd, NULL, strerror(127), false);
}

void	ft_mini_exit_wrapper(t_command *cmd, t_global *g)
{
	if (cmd->args[1] && cmd->args[2])
		return (handle_exit_err(cmd, g, 1));
	else if (cmd->args[1] && !ft_is_numeric_arg(cmd->args[1]))
	{
		handle_exit_err(cmd, g, 2);
		ft_exit(g, cmd->command, 2);
	}
	else if (cmd->args[1])
		ft_exit(g, cmd->command, ft_atoi(cmd->args[1]));
	ft_exit(g, cmd->command, EXIT_SUCCESS);
}

static int	ft_is_numeric_arg(const char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (!ft_isdigit(c[i]) && c[i] != '-' && c[i] != '+')
			return (0);
		i++;
	}
	return (1);
}

static void	handle_exit_err(t_command *cmd, t_global *g, int code)
{
	char	*value;
	char	*final;

	ft_printf("exit\n");
	g->last_exit_code = code;
	value = ft_strjoin("minishell: ", cmd->args[0]);
	if (code == 1)
	{
		final = ft_strjoin(value, ": too many arguments\n");
		ft_putstr_fd(final, 2);
	}
	else if (code == 2)
	{
		final = ft_strjoin(value, ": numeric argument required\n");
		ft_putstr_fd(final, 2);
	}
	free_ptr((void **) &value);
	free_ptr((void **) &final);
}
