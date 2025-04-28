/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:07:42 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:09:52 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_variable(t_token **node)
{
	int	i;

	i = 0;
	while ((*node)->str[i])
	{
		if ((*node)->str[i] == '$')
		{
			if ((*node)->prev && (*node)->prev->type == HEREDOC)
				break ;
			(*node)->type = VAR;
			return ;
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

int	verify_double_separators(t_token **token_lst)
{
	t_token	*temp;

	temp = *token_lst;
	while (temp)
	{
		if (forbidden_consecutives(temp))
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
	if (tmp->type == PIPE)
	{
		write(2, "syntax error\n", 13);
		return (1);
	}
	while (tmp)
	{
		set_variable(&tmp);
		if (verify_double_separators(&tmp))
		{
			write(2, "syntax error\n", 13);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

/*
 parse_user_input:
*	Tokenizes and parses user input into a structure for execution.
*	Returns true if successful, false in case of error.
*/
bool	parse_user_input(t_global *global)
{
	if (global->user_input == NULL)
	{
		exit(0);
		return (false);
	}
	else if (ft_strncmp(global->user_input, "\0", 1) == 0)
		return (false);
	else if (input_is_space(global->user_input))
		return (true);
	add_history(global->user_input);
	if (tokenization(global) != 0)
		return (false);
	if (global->token->type == END)
		return (false);
	if (check_var(&global->token) != 0)
		return (false);
	var_expander(global, &global->token);
	handle_quotes(global);
	create_commands(global, global->token);
	return (true);
}
