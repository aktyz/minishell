/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_ft_get_input_data.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 05:53:41 by zslowian          #+#    #+#             */
/*   Updated: 2024/12/10 13:56:13 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * Function reads data from provided input source and stores it
 * in input_data.
 *
 */
void	ft_get_input_from_fd(t_process **pipex, int fd)
{
	char		*tmp;
	char		*line;
	t_process	*process;

	process = *pipex;
	tmp = NULL;
	process->input_data = NULL;
	line = get_next_line(fd);
	while (line != NULL)
	{
		tmp = process->input_data;
		process->input_data = ft_strreplace(tmp, line);
		if (line)
			free(line);
		line = get_next_line(fd);
	}
	if (line)
		free(line);
}
