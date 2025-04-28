/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_args_echo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:20:04 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* remove_empty_var_args:
**	If a variable does not exist in the environment, the token string
**	will contain "\0". In this case, echo should not print the variable
**	or any spaces before/after it. Therefore the token must be
**	removed before creating/adding echo args.
**	i.e., if variable X does not exist in environment,
**		'echo $X $X $X $USER' should print:
**		'username' (not '  username')
**	However, if the variable exists but its value is empty, the token
**	should not be removed.
*/
void	remove_empty_var_args(t_token **tokens)
{
	t_token	*temp;

	temp = *tokens;
	while (temp->type == WORD || temp->type == VAR)
	{
		if (temp->type == VAR && temp->str[0] == '\0'
			&& temp->var_exists == false)
		{
			temp = temp->next;
			if (temp == (*tokens)->next)
				(*tokens) = (*tokens)->next;
			lstdelone_token(temp->prev, free_ptr);
		}
		else
			temp = temp->next;
	}
}

/*
**  This function counts the number of arguments in the list of tokens
**  To consider a token or multiple tokens as an argument they must be either
**  a WORD or a VAR and if they	temp = *token_node;
 are a VAR that has to be joined, we have
**  to loop through all the tokens that check these conditions
**  (type == VAR and join == true) before counting them as one argument
*/

int	count_args(t_token *temp)
{
	int	i;

	i = 0;
	while (temp && (temp->type == WORD || temp->type == VAR))
	{
		if (temp->type == VAR && temp->join == true)
		{
			while (temp->type == VAR && temp->join == true)
				temp = temp->next;
			i++;
		}
		else
		{
			i++;
			temp = temp->next;
		}
	}
	return (i);
}

/*
**  This function deals with the specific case when the command is "echo"
**    - It allocates the array of arguments thanks to the count_args function
**    - It loops through the tokens list while the nodes are of type
**        VAR or WORD:
**         * If "bool join = true" in the token structure is true : we join all
**           the tokens of type VAR that have the setting "join = true"
**         * if "join = false" we just fill the last_cmd_>args[i]
**            with the current token.
*/
int	create_args_echo_mode(t_token **token_node, t_command *last_cmd)
{
	t_token	*temp;
	int		i;

	remove_empty_var_args(token_node);
	temp = *token_node;
	last_cmd->args_size = count_arguments(temp) + 2;
	last_cmd->args = malloc(sizeof(char *) * (last_cmd->args_size));
	if (!last_cmd->args)
		return (1);
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (temp->type == WORD || temp->type == VAR)
	{
		if (temp->join == true)
			last_cmd->args[i] = join_vars(&temp);
		else
			last_cmd->args[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	last_cmd->args[i] = NULL;
	*token_node = temp;
	return (0);
}

int	add_args_echo_mode(t_token **token_node, t_command *last_cmd)
{
	int		len;
	int		nb_args;
	char	**new_tab;
	t_token	*temp;

	remove_empty_var_args(token_node);
	temp = *token_node;
	nb_args = count_args(temp);
	len = 0;
	while (last_cmd->args[len])
		len++;
	new_tab = malloc(sizeof(char *) * (nb_args + len + 1));
	if (!new_tab)
		return (1);
	new_tab = copy_in_new_tab(len, new_tab, last_cmd, temp);
	free(last_cmd->args);
	last_cmd->args = new_tab;
	while (temp->type == WORD || temp->type == VAR)
		temp = temp->next;
	*token_node = temp;
	return (0);
}
