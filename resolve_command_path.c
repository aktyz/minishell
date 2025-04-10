/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:08:40 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/06 16:22:32 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_count_words(char const *s, char c);

char	*resolve_command_path(char *path, char *cmd)
{
	char	*res;
	char	**candidates;
	char	**i;
	char	*ex;

	res = NULL;
	candidates = ft_split(path, ':');
	if (candidates)
	{
		i = candidates;
		ex = ft_strjoin("/", cmd);
		while (*i)
		{
			res = ft_strjoin(*i, ex);
			if (access(res, X_OK) == -1)
			{
				free(res);
				i++;
			}
			else
				break ;
		}
	}
	if (ex)
		free(ex);
	ft_clear_char_array(&candidates, ft_count_words(path, ':') + 1);
	return (res);
}

static size_t	ft_count_words(char const *s, char c)
{
	size_t	nb_words;
	size_t	i;

	nb_words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			nb_words++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (nb_words);
}
