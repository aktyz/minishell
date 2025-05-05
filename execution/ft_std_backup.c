/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_std_backup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:11:06 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 10:26:40 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_stdout_backup(void);
int	restore_stdout_from_backup(int backup_fd);

int	create_stdout_backup(void)
{
	int	backup_fd;

	backup_fd = dup(STDOUT_FILENO);
	if (backup_fd == -1)
	{
		perror("dup failed");
		return -1;
	}
	return backup_fd;
}

int	restore_stdout_from_backup(int backup_fd)
{
	if	(dup2(backup_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		return -1;
	}
	close(backup_fd);
	return 0;
}
