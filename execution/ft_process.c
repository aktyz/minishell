/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:21:16 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_process(t_global *global);
char		*ft_get_env_var_value(char *env_var_name, t_list *env);
static bool	ft_run_parent_builtins(t_command *cmd, t_global *global);
static void	ft_pipex(t_global *global);
static void	ft_execute(t_global *global, pid_t *last_waited_pid);

void	ft_process(t_global *global)
{
	t_command	*cmd;
	pid_t		last_waited_pid;

	last_waited_pid = -1;
	ft_pipex(global); // here we create all the necessary connections
	ft_execute(global, &last_waited_pid);
	cmd = lst_last_cmd(global->cmd);
	if (cmd->cmd_pid > 0 && cmd->cmd_pid != last_waited_pid)
		waitpid(cmd->cmd_pid, NULL, 0);
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

static bool	ft_run_parent_builtins(t_command *cmd, t_global *global)
{
	if (ft_strncmp(cmd->command, EXIT, ft_strlen(EXIT)) == 0)
	{
		ft_exit(global);
		return (false);
	}
	if (ft_strncmp(cmd->command, CD, ft_strlen(CD)) == 0)
	{
		ft_cd(cmd, global);
		return (false);
	}
	if (ft_strncmp(cmd->command, EXPORT, ft_strlen(EXPORT)) == 0)
	{
		ft_export(cmd, global);
		return (false);
	}
	if (ft_strncmp(cmd->command, UNSET, ft_strlen(UNSET)) == 0)
	{
		ft_unset(cmd, global);
		return (false);
	}
}

static void	ft_pipex(t_global *global)
{
	t_command	*cmd;
	bool		proceed;

	cmd = global->cmd;
	while (cmd)
	{
		proceed = true;
		if (cmd->pipe_output)
			pipe(cmd->pipe_fd);
		if (!cmd->is_builtin)
			cmd->path = resolve_command_path(ft_get_env_var_value(ENV_PATH,
						global->env), cmd->command);
		proceed = ft_run_parent_builtins(cmd, global);
		if (proceed) //TODO @aktyz cmd->cmd_pid = save_fork();
		{
			cmd->cmd_pid = fork();
			if (cmd->cmd_pid == -1)
			{
				perror("Forking failed");
				return ;
			}
		}
		else
			break ;
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

	cmd = global->cmd;
	while (cmd)
	{
		if (cmd->cmd_pid == 0)
			ft_execute_child_proc(cmd, global);
		else if (cmd->cmd_pid > 0 && global->is_global)
		{
			if (!cmd->pipe_output && cmd->cmd_pid > 0)
			{
				waitpid(cmd->cmd_pid, NULL, 0);
				*last_waited_pid = cmd->cmd_pid;
			}
			else if (cmd == global->cmd && cmd->cmd_pid > 0)
				waitpid(cmd->cmd_pid, NULL, WNOHANG);
		}
		cmd = cmd->next;
	}
}
