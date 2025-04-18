/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/13 12:04:21 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_process(t_global *global);

void	ft_process(t_global *global)
{
	t_command	*cmd_i;
	pid_t		last_waited_pid;

	cmd_i = global->cmd;
	while (cmd_i)
	{
		if (cmd_i->pipe_output)
			pipe(cmd_i->pipe_fd);
		if (!cmd_i->is_builtin)
			cmd_i->path = resolve_command_path(getenv(ENV_PATH), cmd_i->command);
		if (ft_strncmp(cmd_i->command, EXIT, 5) == 0)
			ft_exit(global);
		if (ft_strncmp(cmd_i->command, CD, 3) == 0)
		{
			ft_cd(cmd_i);
			break ;
		}
		cmd_i->cmd_pid = fork();
		if (cmd_i->cmd_pid == -1)
		{
			perror("Forking failed");
			return ;
		}
		ft_handle_redirections(cmd_i);
		if (cmd_i->cmd_pid == 0)
		{
			global->is_global = false;
			break;
		}
		cmd_i = cmd_i->next;
	}
	cmd_i = global->cmd;
	last_waited_pid = -1;
	while (cmd_i)
	{
		if (cmd_i->cmd_pid == 0)
		{
			if (cmd_i->prev && cmd_i->prev->cmd_pid)
				waitpid(cmd_i->prev->cmd_pid, NULL, 0);
			if (cmd_i->is_builtin)
			{
				ft_run_builtin(cmd_i, global);
				ft_exit(global);
			}
			else
			{
				execve(cmd_i->path, cmd_i->args, ft_execve_env(global->env));
				perror("execve failed");
				exit(EXIT_FAILURE);
			}
		}
		else if (cmd_i->cmd_pid > 0 && global->is_global)
		{
			if (!cmd_i->pipe_output && cmd_i->cmd_pid > 0)
			{
				waitpid(cmd_i->cmd_pid, NULL, 0);
				last_waited_pid = cmd_i->cmd_pid;
			}
			else if (cmd_i == global->cmd && cmd_i->cmd_pid > 0)
				waitpid(cmd_i->cmd_pid, NULL, WNOHANG);
		}
		cmd_i = cmd_i->next;
	}
	cmd_i = lst_last_cmd(global->cmd);
	if (cmd_i->cmd_pid > 0 && cmd_i->cmd_pid != last_waited_pid)
		waitpid(cmd_i->cmd_pid, NULL, 0);
}
