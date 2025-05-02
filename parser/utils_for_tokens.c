/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_for_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:12:04 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 21:38:43 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_separator(t_token **token, char *str, int index, int type)
{
	int		i;
	char	*separator;

	i = 0;
	if (type == APPEND || type == HEREDOC)
	{
		separator = malloc(sizeof(char) * 3);
		if (!separator)
			return (1);
		while (i < 2)
			separator[i++] = str[index++];
		separator[i] = '\0';
		add_node(token, new_node(separator, type, DEFAULT));
	}
	else
	{
		separator = malloc(sizeof(char) * 2);
		if (!separator)
			return (1);
		while (i < 1)
			separator[i++] = str[index++];
		separator[i] = '\0';
		add_node(token, new_node(separator, type, DEFAULT));
	}
	return (0);
}

static int	add_word(t_token **token, char *str, int index, int start)
{
	int		i;
	char	*word;

	i = 0;
	word = malloc(sizeof(char) * (index - start + 1));
	if (!word)
		return (1);
	while (start < index)
		word[i++] = str[start++];
	word[i] = '\0';
	add_node(token, new_node(word, WORD, DEFAULT));
	return (0);
}

static int	add_word_or_sep(int *i, char *str, int start, t_global *global)
{
	int	type;

	type = which_separator(str, (*i));
	if (type)
	{
		if ((*i) != 0 && which_separator(str, (*i) - 1) == 0)
			add_word(&global->token, str, (*i), start);
		if (type == APPEND || type == HEREDOC || type == PIPE
			|| type == INPUT || type == TRUNC || type == END)
		{
			add_separator(&global->token, str, (*i), type);
			if (type == APPEND || type == HEREDOC)
				(*i)++;
		}
		start = (*i) + 1;
	}
	return (start);
}

static int	set_status(int status, char *str, int i)
{
	if (str[i] == '\'' && status == DEFAULT)
		status = SQUOTE;
	else if (str[i] == '\"' && status == DEFAULT)
		status = DQUOTE;
	else if (str[i] == '\'' && status == SQUOTE)
		status = DEFAULT;
	else if (str[i] == '\"' && status == DQUOTE)
		status = DEFAULT;
	return (status);
}

int	tokenization(t_global *global)
{
	int		i;
	int		start;
	int		status;
	char	*str;

	str = global->user_input;
	i = -1;
	start = 0;
	status = DEFAULT;
	while (++i <= (int)ft_strlen(str))
	{
		status = set_status(status, str, i);
		if (status == DEFAULT)
			start = add_word_or_sep(&i, str, start, global);
	}
	if (status != DEFAULT)
	{
		if (status == DQUOTE)
			printf("Close \"\n");
		else if (status == SQUOTE)
			printf("Close \'\n");
		return (1);
	}
	return (0);
}
