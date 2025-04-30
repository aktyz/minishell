/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:58:06 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/29 16:22:04 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_logs_to_terminal(void);

void	redirect_logs_to_terminal(void)
{
	int	tty_fd;

	tty_fd = open("/dev/tty", O_WRONLY);
	if (tty_fd == -1)
	{
		perror("open");
		return ;
	}
	dup2(tty_fd, STDERR_FILENO);
	dup2(tty_fd, STDOUT_FILENO);
	close(tty_fd);
}
