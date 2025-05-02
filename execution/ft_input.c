/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:48:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 21:30:13 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_attach_tty(t_command *cmd)
{
	int	tty_fd;

	(void)cmd;
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
