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
	pid_t		last_waited_pid;
//****************** To be moved one by one to the while loop below********************/
	// I don't need to close anything because I don't serve pipes yet
/**
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
	if (!access(command->executable->file_name, F_OK & R_OK))
	{
		command->in_file_fd = open(command->executable->file_name, O_RDONLY);
		if (command->in_file_fd == -1)
			ft_error(&proc, NULL);
		dup2(command->in_file_fd, STDIN_FILENO);
		close(command->in_file_fd);
	}
	else
		ft_error(&proc, NULL);*/
	// if there are any pipes(possibly also any redirections) we need to spawn all the processes
	// before the next loop so that all the fds are open for all and the child processes
	// can close/write/read accordingly to their function
	// On the command I would need a pid of the proces spawn, so that the next command
	// can wait for this process to terminate
	//first loop - create pipes/redirections + fork for new processes + handle redirections
	cmd_i = global->cmd;
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
			if (cmd_i->cmd_pid > 0 && cmd_i->prev && cmd_i->prev->pipe_output) // parent process and pipe receiver forked
			{
				close(cmd_i->prev->pipe_fd[0]);
				close(cmd_i->prev->pipe_fd[1]);
			}
			if (cmd_i->cmd_pid == 0) // child
			{
				if((cmd_i->prev && cmd_i->prev->pipe_output) || cmd_i->pipe_output)
					ft_handle_redirections(cmd_i);
				break ;
			}
		}
		cmd_i = cmd_i->next;
	}
	cmd_i = global->cmd;
	last_waited_pid = -1; // Track the last waited PID to avoid duplicate waits
	while (cmd_i) // execution loop
	{
		if (cmd_i->cmd_pid == 0) // Child process
		{
			if (cmd_i->prev && cmd_i->prev->cmd_pid)
				waitpid(cmd_i->prev->cmd_pid, NULL, 0); // Wait for the previous process to finish
			execve(cmd_i->path, cmd_i->args, global->env); // Execute the command
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		else if (cmd_i->cmd_pid > 0 || cmd_i->cmd_pid == -1) // Parent process or built-in cmd
		{
			if (cmd_i->is_builtin)
			{
				if (cmd_i->prev && cmd_i->prev->pipe_output)
					waitpid(cmd_i->cmd_pid, NULL, 0);
				ft_run_builtin(cmd_i);
			}
			if (!cmd_i->pipe_output && cmd_i->cmd_pid > 0)
			{
				waitpid(cmd_i->cmd_pid, NULL, 0); // Wait for the current process if it does not output to a pipe
				last_waited_pid = cmd_i->cmd_pid; // Mark this PID as already waited
			}
			else if (cmd_i == global->cmd && cmd_i->cmd_pid > 0)
				waitpid(cmd_i->cmd_pid, NULL, WNOHANG);// Special case: Wait for the first process to finish setting up redirections
		}

		cmd_i = cmd_i->next;
	}
	// Wait for the last command to finish, but only if it hasn't been waited on
	cmd_i = lst_last_cmd(global->cmd);
	if (cmd_i->cmd_pid > 0 && cmd_i->cmd_pid != last_waited_pid)
		waitpid(cmd_i->cmd_pid, NULL, 0);
}
