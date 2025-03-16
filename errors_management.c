#include "minishell.h"

/* join_strs:
*	Joins two strings together, freeing the previous string.
*	Returns the new concatenated string. Or NULL if an error occured.
*/
char	*join_strs(char *str, char *add)
{
	char	*tmp;

	if (!add)
		return (str);
	if (!str)
		return (ft_strdup(add)); // NOTE: why there is ft_strdup here? not simply returning add?
	tmp = str;
	str = ft_strjoin(tmp, add);
	free_ptr(tmp);
	return (str);
}

/* add_detail_quotes:
*	Checks whether to add quotes around the error detail:
*	i.e. "unset: `@': not a valid identifier"
*	Returns true if the command is export or unset,
*	false if not.
*/
static bool	add_detail_quotes(char *command)
{
	if (ft_strncmp(command, "export", 7) == 0
		|| ft_strncmp(command, "unset", 6) == 0)
		return (true);
	return (false);
}


/* errmsg_cmd:
*	Prints an error message to the standard error, prefixed with the
*	program name.
*	Returns with the specified error number.
*/
int	errmsg_cmd(char *command, char *detail, char *error_message, int error_nb)
{
	char	*msg;
	bool	detail_quotes;

	detail_quotes = add_detail_quotes(command);
	msg = ft_strdup("minishell: ");
	if (command != NULL)
	{
		msg = join_strs(msg, command);
		msg = join_strs(msg, ": ");
	}
	if (detail != NULL)
	{
		if (detail_quotes)
			msg = join_strs(msg, "`");
		msg = join_strs(msg, detail);
		if (detail_quotes)
			msg = join_strs(msg, "'");
		msg = join_strs(msg, ": ");
	}
	msg = join_strs(msg, error_message);
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
	return (error_nb);
}


/* errmsg:
*	Prints an error message that is unrelated to a specific command.
*	Used in parsing phase for syntax errors.
*/
void	errmsg(char *errmsg, char *detail, int quotes)
{
	char	*msg;

	msg = ft_strdup("minishell: ");
	msg = join_strs(msg, errmsg);
	if (quotes)
		msg = join_strs(msg, " `");
	else
		msg = join_strs(msg, ": ");
	msg = join_strs(msg, detail);
	if (quotes)
		msg = join_strs(msg, "'");
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
}


static void	if_variable(t_token **node)
{
	int	i;

	i = 0;
	while ((*node)->str[i]) //checks string in one node
	{
		if ((*node)->str[i] == '$')
		{
			if ((*node)->prev && (*node)->prev->type == HEREDOC) //if '<' in the previous nod
				break;
			(*node)->type = VAR; //if not '<' in the previous node, than change the type for '$' for VAR
			return;
		}
		i++;
	}
}


static bool	forbidden_consecutives(t_token *token_node)
{
	if (token_node->prev)
	{
		if (token_node->type == PIPE && token_node->prev->type == PIPE)
			return (true);
		if (token_node->type > PIPE && token_node->prev->type > PIPE)
			return (true);
		if (token_node->type == END && token_node->prev->type >= PIPE)
			return (true);
	}
	return (false);
}


// TODO rename this
int	check_consecutives(t_token **token_lst)
{
	t_token	*temp;

	temp = *token_lst;
	while (temp)
	{
		if (forbidden_consecutives(temp) == true)
		{
			if (temp->type == END && temp->prev && temp->prev->type > PIPE)
				write(2, "syntax error\n", 13);
			else if (temp->type == END && temp->prev)
				write(2, "syntax error\n", 13);
			else
				write(2, "syntax error\n", 13);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

int	check_var(t_token **token_lst)
{
	t_token	*tmp;
	
	tmp = *token_lst;
	if (tmp->type == PIPE) //the first node cannot be PIPE
	{
		write(2, "syntax error\n", 13);
		return (1);
	}
	while (tmp) //now we check the list of tokens
	{
		if_variable(&tmp);
		if (check_consecutives(&tmp))
		{
			write(2, "syntax error\n", 13);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	char_isspace(int c)
{
	if (c == 32 || (c > 8 && c < 14))
		return (1);
	else
		return (0);
}

bool	input_is_space(char *input) //for checking if the input is only space
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


/*
 parse_user_input:
*	Tokenizes and parses user input into a structure for execution.
*	Returns true if successful, false in case of error.
*/
bool	parse_user_input(t_global *global)
{
	if (global->user_input == NULL) // readline function OR malloc inside ft_strdup function did not work
		return false;
		//exit_builtin(global, NULL); // NOTE: builtin not implemented
	else if (ft_strncmp(global->user_input, "\0", 1) == 0) // empty string in user_input
		return (false);
	else if (input_is_space(global->user_input)) // only whitespace characters in user_input
		return (true);
	add_history(global->user_input);
	if (tokenization(global) != 0) // lexer returned an error
		return (false);
	if (global->token->type == END) // first token has type END, meaning no token was created by lexer
		return (false);
	if (check_var(&global->token) != 0)
		return (false);
	var_expander(global, &global->token);
	// handle_quotes(global);
	// create_commands(global, global->token);
	return (true);
}

