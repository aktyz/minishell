/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 18:07:40 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_process(t_global *global);
void		ft_run_parent_builtins(t_command *cmd, t_global *global);
static void	ft_execute_parent_built_in(t_global *g, t_command *cmd);
static void	ft_pipex(t_global *global);
static void	ft_execute(t_global *global);

void	ft_process(t_global *global)
{
	if (global->cmd && global->cmd->content)
	{
		ft_pipex(global);
		ft_execute(global);
	}
}

void	ft_run_parent_builtins(t_command *cmd, t_global *g)
{
	int	check;

	check = 0;
	ft_execute_parent_built_in(g, cmd);
	if (cmd->pipe_output || (cmd->final_io && cmd->final_io->outfile))
	{
		check = restore_stdout_from_backup(cmd->stdout_backup);
		if (check)
		{
			ft_minishell_perror("parent failed to restore stdout backup", 1);
			ft_exit(g, "parent failed to restore stdout backup", 1);
		}
	}
}

static void	ft_execute_parent_built_in(t_global *g, t_command *cmd)
{
	if (cmd->command && ft_strncmp(cmd->command, EXIT, ft_strlen(EXIT)) == 0)
		ft_mini_exit_wrapper(cmd, g);
	else if (cmd->command && ft_strncmp(cmd->command, CD, ft_strlen(CD)) == 0)
		g->last_exit_code = ft_cd(cmd, g);
	else if (cmd->command && ft_strncmp(cmd->command,
			EXPORT, ft_strlen(EXPORT)) == 0)
		ft_mini_export_wrapper(cmd, g);
	else if (cmd->command && ft_strncmp(cmd->command,
			UNSET, ft_strlen(UNSET)) == 0)
		ft_unset(cmd, g);
	else if (cmd->status_request)
	{
		ft_printf("%s %s: command not found\n", MINISHELL, cmd->command);
		g->last_exit_code = 127;
	}
}

static void	ft_pipex(t_global *g)
{
	t_command	*curr_cmd;
	t_list		*ptr;
	t_command	*prev_cmd;

	ptr = g->cmd;
	prev_cmd = NULL;
	while (ptr)
	{
		curr_cmd = (t_command *) ptr->content;
		if (curr_cmd->pipe_output)
			pipe(curr_cmd->pipe_fd);
		ft_split_child_parent_run(g, curr_cmd, prev_cmd);
		if (curr_cmd->cmd_pid == 0)
			break ;
		prev_cmd = curr_cmd;
		ptr = ptr->next;
	}
}

static void	ft_execute(t_global *global)
{
	t_command	*curr_cmd;
	pid_t		prev_pid;
	t_list		*ptr;

	ptr = global->cmd;
	prev_pid = -1;
	while (ptr)
	{
		curr_cmd = (t_command *) ptr->content;
		ft_execute_cmd(global, curr_cmd, prev_pid);
		prev_pid = curr_cmd->cmd_pid;
		ptr = ptr->next;
	}
}
