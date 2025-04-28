/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:20:04 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_arguments(t_token *temp)
{
	int	i;

	i = 0;
	while (temp && (temp->type == WORD || temp->type == VAR))
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

/*
	TRUNC -> Redirection of output.
	The file whose name results from the expansion of word has to be opened
    in writing mode on fd n or the standard output (fd 1) if n is not specified.
    If the file does not exist it is created;
    if it does exist it is truncated to 0 size.

	The general format for redirecting output is: [n]>[|]word
*/

char	*get_relative_path(char *file_to_open)
{
	char	*path;
	char	*ret;

	if (file_to_open[0] == '/')
		return (ft_strdup(file_to_open));
	path = ft_strdup("./");
	ret = ft_strjoin(path, file_to_open);
	printf("PARSING - Get_rel_path function return : %s\n", ret);
	return (ret);
}

bool	contains_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (true);
		i++;
	}
	return (false);
}
