#include "minishell.h"


bool	is_variable_character(char c)
{
	return ft_isalnum(c) || c == '_';
}

/* returns number of characters in the first encountered variable that starts with $ 

*/
int	var_length(char *str)
{
	int		i;
	int		count;

	count = 0;
	i = 0;
	while (str[i] != '$')
		i++;
	i++;
	if ((str[i] >= '0' && str[i] <= '9') || str[i] == '?')
		return (count + 1);
	while (str[i])
	{
		if (!is_variable_character(str[i]))
			break ;
		count++;
		i++;
	}
	return (count);
}


void	copy_var_value(char *new_str, char *var_value, int *j)
{
	int	k;

	k = 0;
	while (var_value[k])
	{
		new_str[*j] = var_value[k];
		k++;
		(*j)++;
	}
}

char	*get_new_token_string(char *oldstr, char *var_value,
		int newstr_size, int index)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * newstr_size);
	if (!new_str)
		return (NULL);
	while (oldstr[i])
	{
		if (oldstr[i] == '$' && i == index)
		{
			copy_var_value(new_str, var_value, &j);
			i = i + var_length(oldstr + index) + 1;
			if (oldstr[i] == '\0')
				break ;
		}
		new_str[j++] = oldstr[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

// Changed return type from int to char * to adapt the function
// to work for heredoc variable expansion. Heredoc has no tokens
// so token_node becomes optional.
// Heredoc variant replace_str_heredoc calls this function with
// token_node == NULL!

static char	*erase_and_replace(t_token **token_node, char *str,
			char *var_value, int index)
{
	char	*newstr;
	int		newstr_size;

	newstr_size = (ft_strlen(str) - var_length(str + index)
			+ ft_strlen(var_value));
	newstr = get_new_token_string(str, var_value, newstr_size, index);
	if (token_node && *token_node)
	{
		free_ptr((*token_node)->str);
		(*token_node)->str = newstr;
	}
	return (newstr);
}

int	replace_var(t_token **token_node, char *var_value, int index)
{
	// NOTE: removed erase_var as it looks like a special case for replace with empty string.
	if (var_value == NULL) {
		var_value = malloc(1);
		var_value[0] = '\0';
	}
	if (erase_and_replace(token_node, (*token_node)->str, \
	var_value, index) == NULL)
	{
		free_ptr(var_value);
		return (1);
	}
	free_ptr(var_value);
	return (0);
}

/* replace_str_heredoc:
*	Heredoc variant of replace_var, replaces an environment variable
*	by its value. Ex. $USER -> username.
*	Returns the replaced string.
*/

char	*replace_str_heredoc(char *str, char *var_value, int index)
{
	char	*tmp;

	tmp = NULL;
	if (var_value == NULL)
		*str = '\0';
	else
	{
		tmp = str;
		str = erase_and_replace(NULL, str, var_value, index);
		free_ptr(tmp);
	}
	free_ptr(var_value);
	return (str);
}




char	*identify_var(char *str)
{
	char	*var;
	char	*tmp;
	int		start;
	int		len;
	int		i;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			start = i + 1;
			break ;
		}
		i++;
	}
	len = var_length(str);
	var = ft_substr(str, start, len);
	if (!var)
		return (NULL);
	tmp = ft_strjoin(var, "=");
	free_ptr(var);
	var = tmp;
	return (var);
}

/*
*  After splitting the user's input into tokens, we have to expand 
*  the variables. After the expansion is done, quote characters are
*  removed from the original word unless they are between quotes.
*/


static int	var_exists(t_global *global, char *var)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (global->env[i])
	{
		if (ft_strncmp(global->env[i], var, len) == 0)
			return (0);
		i++;
	}
	return (1);
}

static char	*search_env_var(t_global *global, char *var)
{
	char	*str;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (global->env[i])
	{
		if (ft_strncmp(global->env[i], var, len) == 0)
			break ;
		i++;
	}
	str = ft_strdup(global->env[i] + len);
	return (str);
}

char	*recover_val(t_token *token, char *str, t_global *global)
{
	char	*value;
	char	*var;

	var = identify_var(str);
	if (var && var_exists(global, var) == 0)
	{
		if (token != NULL)
			token->var_exists = true;
		value = search_env_var(global, var);
	}
	else if (var && var[0] == '?' && var[1] == '=')
		value = ft_itoa(g_last_exit_code);
	else
		value = NULL;
	free_ptr(var);
	return (value);
}

static void	update_status(t_token **token_node, char c)
{
	if (c == '\'' && (*token_node)->status == DEFAULT)
		(*token_node)->status = SQUOTE;
	else if (c == '\"' && (*token_node)->status == DEFAULT)
		(*token_node)->status = DQUOTE;
	else if (c == '\'' && (*token_node)->status == SQUOTE)
		(*token_node)->status = DEFAULT;
	else if (c == '\"' && (*token_node)->status == DQUOTE)
		(*token_node)->status = DEFAULT;
}

static bool	is_next_char_a_sep(char c)
{
	return (c == '$' || c == ' ' || c == '=' || c == '\0');
}

static bool	var_between_quotes(char *str, int i)
{
	return (i > 0 && str[i - 1] == '\"' && str[i + 1] == '\"');
}

// NOTE: why both global is passed and token_lst?
// cannot simply take global->token and have this as one argument function?
// TODO add test cases for this
int	var_expander(t_global *global, t_token **token_lst)
{
	t_token	*temp;
	int		i;

	temp = *token_lst;
	while (temp)
	{
		if (temp->type == VAR)
		{
			i = 0;
			while (temp->str[i])
			{
				update_status(&temp, temp->str[i]);
				if (temp->str[i] == '$'
					&& !is_next_char_a_sep(temp->str[i + 1])
					&& !var_between_quotes(temp->str, i)
					&& (temp->status == DEFAULT || temp->status == DQUOTE))
					replace_var(&temp,
						recover_val(temp, temp->str + i, global), i);
				else
					i++;
			}
		}
		temp = temp->next;
	}
	return (0);
}

/* var_expander_heredoc:
*	Heredoc variant of var_expander. Replaces a string containing an
*	environment variable, like $USER with its corresponding value.
*	Returns the replaced string. May return NULL on error.
*/
char	*var_expander_heredoc(t_global *global, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$'
			&& !is_next_char_a_sep(str[i + 1])
			&& !var_between_quotes(str, i))
			str = replace_str_heredoc(str, recover_val(NULL, str + i, global), i);
		else
			i++;
	}
	return (str);
}