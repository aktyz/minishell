/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_input.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:48:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 18:52:48 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_attach_tty(t_command *cmd)
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
