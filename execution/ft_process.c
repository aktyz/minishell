/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:11:29 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/11 22:55:30 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_process(t_global *global);

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
	t_command	*cmd_i;
	bool		is_builtin;
//****************** To be moved one by one to the while loop below********************/
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
	cmd_i = global->cmd;

	// if there are any pipes(possibly also any redirections) we need to spawn all the processes
	// before the next loop so that all the fds are open for all and the child processes
	// can close/write/read accordingly to their function
	// On the command I would need a pid of the proces spawn, so that the next command
	// can wait for this process to terminate
	//first loop - create pipes/redirections + fork for new processes
	while (cmd_i)
	{
		if (cmd_i->pipe_output)
			pipe(cmd_i->pipe_fd); // Create a pipe for the current command

		if (!cmd_i->is_builtin)
		{
			cmd_i->path = resolve_command_path(extract_env_var(ENV_PATH, global->env), cmd_i->command);
			cmd_i->cmd_pid = fork();
			if (cmd_i->cmd_pid == -1)
			{
				perror("Forking failed");
				return;
			}
			if (cmd_i->cmd_pid == 0)
				break ;
		}
		cmd_i = cmd_i->next;
	}

	// Second loop: Handle redirections and execute commands
	cmd_i = global->cmd;
	pid_t last_waited_pid = -1; // Track the last waited PID to avoid duplicate waits

	while (cmd_i)
	{
		if (cmd_i->cmd_pid == 0) // Child process
		{
			if (cmd_i->prev && cmd_i->prev->cmd_pid)
			{
				// Wait for the previous process to finish writing to the pipe
				waitpid(cmd_i->prev->cmd_pid, NULL, 0);
			}

			// Handle redirections for the current command
			ft_handle_redirections(cmd_i);

			// Execute the command
			execve(cmd_i->path, cmd_i->args, global->env);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		else if (cmd_i->cmd_pid > 0) // Parent process
		{
			if (cmd_i->pipe_output)
			{
				close(cmd_i->pipe_fd[0]);
				close(cmd_i->pipe_fd[1]);
			}
			if (!cmd_i->pipe_output)
			{
				// Wait for the current process if it does not output to a pipe
				waitpid(cmd_i->cmd_pid, NULL, 0);
				last_waited_pid = cmd_i->cmd_pid; // Mark this PID as already waited
			}
			else if (cmd_i == global->cmd)
			{
				// Special case: Wait for the first process to finish setting up redirections
				waitpid(cmd_i->cmd_pid, NULL, WNOHANG);
			}
		}
		cmd_i = cmd_i->next;
	}

	// Wait for the last command to finish, but only if it hasn't been waited on
	cmd_i = lst_last_cmd(global->cmd);
	if (cmd_i->cmd_pid > 0 && cmd_i->cmd_pid != last_waited_pid)
		waitpid(cmd_i->cmd_pid, NULL, 0);
}
