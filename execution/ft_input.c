/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:48:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 19:18:02 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_attach_tty(void);
void	ft_calloc_io_node(t_io_fds **ptr, t_global *g);
void	ft_execute_cmd(t_global *g, t_command *cmd, pid_t prev_pid);
void	ft_check_path(char *path, int *error);

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
	else
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

	if (!access(path, F_OK))
		*error = 128;
	if (!stat(path, &info))
		*error = 128;
	if (S_ISDIR(info.st_mode))
		*error = EISDIR;
	if (!*error)
	{
		if (!access(path, X_OK))
			*error = 2;
		if (!access(path, R_OK))
			*error = 4;
		if (!access(path, W_OK))
			*error = 8;
	}
}
