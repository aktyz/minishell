/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_mocks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:16:07 by zslowian          #+#    #+#             */
/*   Updated: 2025/03/12 13:49:37 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	one_command(t_command **command)
{
	t_command	*cmd;
	char		**i;

	cmd = ft_calloc(1, sizeof(t_command));
	cmd->command = "ls";
	cmd->path = ft_strjoin(PATH_1, "ls");
	if (access(cmd->path, X_OK) == -1)
	{
		free(cmd->path);
		cmd->path = ft_strjoin(PATH_2, "ls");
	}
	cmd->args = ft_calloc(3, sizeof(char *));
	i = cmd->args;
	*i = "ls";
	i++;
	*i = "-la";
	i++;
	*i = NULL;
	command = &cmd;
}