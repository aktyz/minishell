/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:48:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 18:11:44 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_attach_tty(void);
void	ft_calloc_io_node(t_io_fds **ptr, t_global *g);
void	ft_execute_cmd(t_global *g, t_command *cmd, pid_t prev_pid);
void	ft_check_path(char *path, int *error);
void	ft_command_not_found(char *path, int *error);

void	ft_attach_tty(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd == -1)
	{
		perror(MINISHELL);
		return ;
	}
	if (dup2(tty_fd, STDIN_FILENO) == -1)
	{
		perror(MINISHELL);
		close(tty_fd);
		return ;
	}
	close(tty_fd);
}

void	ft_calloc_io_node(t_io_fds **ptr, t_global *g)
{
	*ptr = ft_calloc(sizeof(t_io_fds), 1);
	if (!(*ptr))
	{
		ft_minishell_perror("ft_calloc_io_node", errno);
		ft_exit(g, "ft_calloc_io_node", 1);
	}
	(*ptr)->fd_in = -1;
	(*ptr)->fd_out = -1;
}

void	ft_execute_cmd(t_global *g, t_command *cmd, pid_t prev_pid)
{
	int	wstatus;

	wstatus = 0;
	if (cmd->cmd_pid == 0)
		ft_execute_child_proc(cmd, g, prev_pid);
	else if (g->is_global)
	{
		if (cmd->cmd_pid == -1)
			ft_run_parent_builtins(cmd, g);
		else
		{
			waitpid(cmd->cmd_pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
				g->last_exit_code = WEXITSTATUS(wstatus);
		}
	}
}

void	ft_check_path(char *path, int *error)
{
	struct stat	info;
	bool		dir_or_cmd;

	dir_or_cmd = (ft_strchr(path, '/') || path[0] == '.');
	if (stat(path, &info) != 0)
		*error = 127;
	if (*error && dir_or_cmd)
		return (ft_handle_minishell_err(path, ": No such file or directory\n"));
	if (*error)
	{
		if (dir_or_cmd)
		{
			*error = 126;
			return (ft_handle_minishell_err(path,
					": No such file or directory\n"));
		}
		else
			return (ft_handle_minishell_err(path, ": command not found\n"));
	}
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
	if (S_ISDIR(info.st_mode))
	{
		*error = 126;
		if (dir_or_cmd)
			return (ft_handle_minishell_err(path, ": Is a directory\n"));
		*error = 127;
		return (ft_handle_minishell_err(path, ": command not found\n"));
	}
}

void	ft_command_not_found(char *path, int *error)
{
	if (access(path, F_OK | X_OK))
	{
		*error = 127;
		ft_handle_minishell_err(path, ": command not found\n");
	}
}
