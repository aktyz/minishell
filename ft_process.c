/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/05 20:07:47 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_clean_command(t_command *cmd);
void		ft_process(t_global *global);

/**
 * Function that will eventually call our executable.
 * It contains three parts:
 * - setting up the STDOUT for the executable
 * - setting up the STDIN for the executable
 * - executing using execve() function
 *
 */
void	ft_process(t_global *global)
{
	pid_t	child_proc;

	global->cmd = ft_calloc(1, sizeof(t_command)); // TODO: remove when merging with Marlena's work
	test_single_cmd(global); // populates cmd list with single command test cases
	
	// I don't need to close anything because I don't serve pipes yet
	/**if (command->pipe_send)
	{
		close(command->pipe_parent[0]);
		dup2(command->pipe_parent[1], STDOUT_FILENO);
		close(command->pipe_parent[1]);
	}
	else
	{
		close(command->pipe_parent[1]);
		command->out_file_fd = open(command->executable->file_name, O_RDWR | O_CREAT, 0644);
		if (command->out_file_fd == -1)
			ft_error(&proc, NULL);
		if (!access(command->executable->file_name, F_OK))
		{
			dup2(command->out_file_fd, STDOUT_FILENO);
			close(command->out_file_fd);
		}
		else
			ft_error(&proc, NULL);
	}
	if (command->pipe_receive)
	{
		dup2(command->pipe_parent[0], STDIN_FILENO);
		close(command->pipe_parent[0]);
	}
	else if (!access(command->executable->file_name, F_OK & R_OK))
	{
		command->in_file_fd = open(command->executable->file_name, O_RDONLY);
		if (command->in_file_fd == -1)
			ft_error(&proc, NULL);
		dup2(command->in_file_fd, STDIN_FILENO);
		close(command->in_file_fd);
	}
	else
		ft_error(&proc, NULL);*/
	while (global->cmd)
	{
		child_proc = fork();
		if (child_proc == -1)
			ft_printf("Forking failed\n");
		if(!child_proc)
			execve(global->cmd->path, global->cmd->args, NULL);
		if(global->cmd->next)
		{
			global->cmd = global->cmd->next;
			if (global->cmd->prev)
				ft_clean_command(global->cmd->prev);
		}
		else
		{
			ft_clean_command(global->cmd);
			break;
		}
		waitpid(child_proc, NULL, 0);
	}
}

static void	ft_clean_command(t_command *cmd)
{
	ft_clear_char_array(&cmd->args, cmd->args_size);
	if (cmd)
		free(cmd);
	cmd = NULL;
}
