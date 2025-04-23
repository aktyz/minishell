/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   characters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:07:42 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:09:52 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	char_isspace(int c)
{
	if (c == 32 || (c > 8 && c < 14))
		return (1);
	else
		return (0);
}

bool	input_is_space(char *input) // for checking if the input is only space
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!char_isspace(input[i]))
			return (false);
		i++;
	}
	return (true);
}

int	which_separator(char *str, int i)
{
	if ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		return (SPACES);
	else if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '<')
		return (INPUT);
	else if (str[i] == '>')
		return (TRUNC);
	else if (str[i] == '\0')
		return (END);
	else
		return (0);
}
