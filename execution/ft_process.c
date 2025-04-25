/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 20:38:13 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_process(t_global *global);
char		*ft_get_env_var_value(char *env_var_name, t_list *env);
static void	ft_run_parent_builtins(t_command *cmd, t_global *global);
static void	ft_pipex(t_global *global);
static void	ft_execute(t_global *global, pid_t *last_waited_pid);

void	ft_process(t_global *global)
{
	t_command	*cmd;
	pid_t		last_waited_pid;
	int			wstatus;

	last_waited_pid = -1;
	if (global->cmd->command)
	{
		ft_pipex(global);
		ft_execute(global, &last_waited_pid);
		cmd = lst_last_cmd(global->cmd);
		if (cmd->cmd_pid > 0 && cmd->cmd_pid != last_waited_pid)
			waitpid(cmd->cmd_pid, &wstatus, 0);
		if (WIFEXITED(wstatus))
			global->last_exit_code = WEXITSTATUS(wstatus);
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

static void	ft_run_parent_builtins(t_command *cmd, t_global *global)
{
	ft_handle_redirections(cmd);
	if (ft_strncmp(cmd->command, EXIT, ft_strlen(EXIT)) == 0)
	{
		ft_exit(global, 0);
		return ;
	}
	if (ft_strncmp(cmd->command, CD, ft_strlen(CD)) == 0)
	{
		global->last_exit_code = ft_cd(cmd, global);
		return ;
	}
	if (ft_strncmp(cmd->command, EXPORT, ft_strlen(EXPORT)) == 0)
	{
		ft_export(cmd, global);
		return ;
	}
	if (ft_strncmp(cmd->command, UNSET, ft_strlen(UNSET)) == 0)
	{
		ft_unset(cmd, global);
		return ;
	}
	if (ft_strncmp(cmd->command, ZERO, ft_strlen(ZERO)) == 0)
		ft_printf("%d\n", global->last_exit_code);
}

static void	ft_pipex(t_global *global)
{
	t_command	*cmd;

	cmd = global->cmd;
	while (cmd)
	{
		if (cmd->pipe_output)
			pipe(cmd->pipe_fd);
		if (!cmd->is_builtin)
			cmd->path = resolve_command_path(ft_get_env_var_value(ENV_PATH,
						global->env), cmd->command);
		if (is_parent_builtin(cmd->command))
			ft_run_parent_builtins(cmd, global);
		else
			ft_safe_fork(global, cmd);
		ft_handle_redirections(cmd);
		if (cmd->cmd_pid == 0)
		{
			global->is_global = false;
			break ;
		}
		cmd = cmd->next;
	}
} // Norm: Function too long

static void	ft_execute(t_global *global, pid_t *last_waited_pid)
{
	t_command	*cmd;
	int			wstatus;

	cmd = global->cmd;
	while (cmd)
	{
		if (cmd->cmd_pid == 0)
			ft_execute_child_proc(cmd, global);
		else if (cmd->cmd_pid > 0 && global->is_global)
		{
			if (!cmd->pipe_output && cmd->cmd_pid > 0)
			{
				waitpid(cmd->cmd_pid, &wstatus, 0);
				*last_waited_pid = cmd->cmd_pid;
			}
			else if (cmd == global->cmd && cmd->cmd_pid > 0)
				waitpid(cmd->cmd_pid, &wstatus, WNOHANG);
			if (WIFEXITED(wstatus))
				global->last_exit_code = WEXITSTATUS(wstatus);
		}
		cmd = cmd->next;
	}
}
