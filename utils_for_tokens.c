#include "minishell.h"

t_token	*new_node(char *str, int type, int status)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->str = str;
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	add_node(t_token **list, t_token *new_node)
{
	t_token	*start;

	start = *list;
	if (start == NULL)
	{
		*list = new_node;
		return ;
	}
	if (list && *list && new_node)
	{
		while (start->next != NULL)
		{
			start = start->next;
		}
		start->next = new_node;
		new_node->prev = start;
	}
}

void	delete_node(t_token *node, void (*del)(void *))
{
	if (del && node && node->str)
	{
		(*del)(node->str);
		node->str = NULL;
	}
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	//free_node(node);
}

int	add_separator(t_token **token, char *str, int index, int type)
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

int	add_word(t_token **token, char *str, int index, int start)
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

int	which_separator(char *str, int i)
{
	if ((str[i] > 8 && str[i] < 14 || str[i] == 32))
		return (SPACES);
	else if (str[i] == '|')
		return (PIPE);
	else if (str[i] == '<')
		return (INPUT);
	else if (str[i] == '>')
		return (TRUNC);
	else if (str[i] == '<' && str[i + 1] == '<')
		return (HEREDOC);
	else if (str[i] == '>' && str[i + 1] == '>')
		return (APPEND);
	else if (str[i] == '\0')
		return (END);
	else
		return (0);
}

int	add_word_or_sep(int *i, char *str, int start, t_global *global)
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

int	set_status(int status, char *str, int i)
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
	while (++i <= ft_strlen(str))
	{
		status = set_status(status, str, i);
		if (status == DEFAULT)
			start = add_word_or_sep(&i, str, start, global);
	}
	if (status != DEFAULT)
	{
		if (status == DQUOTE)
			printf("Close \"");
		else if (status == SQUOTE)
			printf("Close \'");
		return (1);
	}
	return (0);
}
