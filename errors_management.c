#include "minishell.h"

void    if_variable(t_token **node)
{
    int i;

    i = 0;
    while ((*node->str[i])) //checks string in one node 
    {
        if (*node->str[i] == '$')
        {
            if(*node->prev && *node->prev->type == HEREDOC) //if '<' in the previous node
                break;
            *node->type = VAR; //if not '<' in the previous node, than change the type for '$' for VAR
            return;
        }
        i++;
    }
}

static bool    forbidden_consecutives(t_token *token_node)
{
    if (token_node->prev)
    {
        if (token_node->type == PIPE && token_node->prev->type == PIPE)
            return (TRUE);
        if (token_node->type > PIPE && token_node->prev->type > PIPE)
            return (TRUE);
        if (token_node->type == END && token_node->prev->type >= PIPE)
            return (TRUE);
    }
    return (FALSE);
}

int     check_var(t_token **token_lst)
{
    t_token *tmp;

    tmp = *token_lst;
    if (tmp->type == PIPE) //the first node cannot be PIPE
    {
        write(2, "syntax error\n", 13);
        return (FAILUER);
    }
    while (tmp) //now we check the list of tokens
    {
        if_variable(&tmp);
        if (forbidden_consecutives(&tmp))
        {
            write(2, "syntax error\n", 13);
            return (FAILURE);
        }
            
        tmp = tmp->next;
    }
    return (SUCCESS);
}
static bool	input_is_space(char *input) //for checking if the input is only space
{
    int	i;
    
    i = 0;
    while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (false);
		i++;
	}
	return (true);
}
/*
 parse_user_input:
*	Tokenizes and parses user input into a structure for execution.
*	Returns true if successful, false in case of error.

bool	parse_user_input(t_data *data)
{
	if (data->user_input == NULL) // readline function OR malloc inside ft_strdup function did not work
		exit_builtin(data, NULL);
	else if (ft_strcmp(data->user_input, "\0") == 0) // empty string in user_input
		return (false);
	else if (input_is_space(data->user_input)) // only whitespace characters in user_input
		return (true);
	add_history(data->user_input);
	if (tokenization(data, data->user_input) == FAILURE) // lexer returned an error
		return (false);
	if (data->token->type == END) // first token has type END, meaning no token was created by lexer
		return (false);
	if (check_if_var(&data->token) == FAILURE)
		return (false);
	var_expander(data, &data->token);
	handle_quotes(data);
	create_commands(data, data->token);
	return (true);
}
*/