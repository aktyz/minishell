/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/29 16:22:43 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_process(t_global *global);
char		*ft_get_env_var_value(char *env_var_name, t_list *env);
void		ft_run_parent_builtins(t_command *cmd, t_global *global);
static void	ft_pipex(t_global *global);
static void	ft_execute(t_global *global);

void	ft_process(t_global *global)
{
	if (global->cmd && global->cmd->command)
	{
		ft_pipex(global);
		ft_execute(global);
	}
}

char	*ft_get_env_var_value(char *env_var_name, t_list *env)
{
	t_minishell_env	*content;

	while (env)
	{
		content = (t_minishell_env *)env->content;
		if (ft_strcmp(env_var_name, content->name_value[0]) == 0)
			return (content->name_value[1]);
		env = env->next;
	}
	return (NULL);
}

void	ft_run_parent_builtins(t_command *cmd, t_global *global)
{
	if (cmd->command && ft_strncmp(cmd->command, EXIT, ft_strlen(EXIT)) == 0)
		ft_mini_exit_wrapper(cmd, global);
	else if (cmd->command && ft_strncmp(cmd->command, CD, ft_strlen(CD)) == 0)
		global->last_exit_code = ft_cd(cmd, global);
	else if (cmd->command && ft_strncmp(cmd->command,
			EXPORT, ft_strlen(EXPORT)) == 0)
		ft_mini_export_wrapper(cmd, global);
	else if (cmd->command && ft_strncmp(cmd->command,
			UNSET, ft_strlen(UNSET)) == 0)
		ft_unset(cmd, global);
	else if (cmd->status_request)
	{
		ft_printf("%s %s: command not found\n", MINISHELL, cmd->command);
		global->last_exit_code = 127;
	}
}

static void	ft_pipex(t_global *g)
{
	t_command	*cmd;

	cmd = g->cmd;
	while (cmd)
	{
		if (cmd->pipe_output)
			pipe(cmd->pipe_fd);
		if (!cmd->is_builtin)
			cmd->path = resolve_command_path(g,
					ft_get_env_var_value(ENV_PATH, g->env), cmd->command);
		ft_split_child_parent_run(g, cmd);
		if (cmd->cmd_pid == 0)
		{
			g->is_global = false;
			break ;
		}
		cmd = cmd->next;
	}
}

static void	ft_execute(t_global *global)
{
	t_command	*cmd;
	int			wstatus;

	cmd = global->cmd;
	wstatus = 0;
	while (cmd)
	{
		if (cmd->cmd_pid == 0)
			ft_execute_child_proc(cmd, global);
		else
		{
			if (cmd->cmd_pid == -1)
				ft_run_parent_builtins(cmd, global);
			else
			{
				waitpid(cmd->cmd_pid, &wstatus, 0);
				if (WIFEXITED(wstatus))
					global->last_exit_code = WEXITSTATUS(wstatus);
			}
		}
		cmd = cmd->next;
	}
}
