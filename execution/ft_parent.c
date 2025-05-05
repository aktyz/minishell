/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parent.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:19:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 19:00:22 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_prepare_parent_process_fds(t_global *g, t_command *cmd,
			t_command *prev_cmd)
{
	if (prev_cmd && prev_cmd->cmd_pid == -1 && prev_cmd->pipe_output)
	{
		prev_cmd->stdout_backup = create_stdout_backup();
		if (prev_cmd->stdout_backup)
			ft_exit(g, "failed to create stdout backup", 1);
		close(prev_cmd->pipe_fd[0]);
		dup2(prev_cmd->pipe_fd[1], STDOUT_FILENO);
		close(prev_cmd->pipe_fd[1]);
	}
	else if (prev_cmd && prev_cmd->pipe_output)
	{
		close(prev_cmd->pipe_fd[0]);
		close(prev_cmd->pipe_fd[1]);
	}
	if (cmd->final_io && cmd->final_io->outfile)
	{
		cmd->stdout_backup = create_stdout_backup();
		ft_open_final_outfile(g, &cmd->final_io);
	}
}

void	ft_check_error(char *path, int *error, bool *dir_or_cmd)
{
	if (*error && *dir_or_cmd)
		return (ft_handle_minishell_err(path, ": No such file or directory\n"));
	if (*error)
	{
		if (*dir_or_cmd)
		{
			*error = 126;
			return (ft_handle_minishell_err(path,
					": No such file or directory\n"));
		}
		else
			return (ft_handle_minishell_err(path, ": command not found\n"));
	}
}

void	ft_check_error2(char *path, int *error, bool *dir_or_cmd)
{
	if (access(path, F_OK))
	{
		*error = 127;
		ft_minishell_perror(path, ENOENT);
	}
	else if (access(path, X_OK))
	{
		*error = 126;
		if (dir_or_cmd)
			ft_minishell_perror(path, EACCES);
		else
		{
			*error = 127;
			return (ft_handle_minishell_err(path, ": command not found\n"));
		}
	}
}
