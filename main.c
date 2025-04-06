/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/01 16:14:58 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_clean_up(t_global **proc);
void		ft_error(t_global ***proc, char **string);
void		print_tokens(t_token *list);
void		minishell_interactive(t_global *global);




// NOTE: comment this function to run test cases from tests.c
// criterion test runner adds its own main function

/**/
int	main(int ac, char **av, char **env)
{

	t_global	global; // NOTE originally data

	test_ft_echo();
	if (!init_global(&global, env))
		exit_shell(NULL, EXIT_FAILURE);
	minishell_interactive(&global);
	return (0);
}
/**/

void	print_tokens(t_token *list)
{
	t_token	*temp;

	temp = list;
	while (temp)
	{
		printf("%s \n", temp->str);
		temp = temp->next;
	}
}

void	minishell_interactive(t_global *global)
{
	while (1)
	{
		set_signals_interactive();
		global->user_input = readline(PROMPT);
		set_signals_noninteractive();
		if (parse_user_input(global)) {
			printf("parsed user input\n");
			print_token_list(&global->token);
		}
			//g_last_exit_code = execute(global);
		//else
		//	g_last_exit_code = 1;

		free_global(global, false);
		global->token = NULL;
		/*
		print_tokens(global->token);
		*/
		// ft_process(global);
	}
}

void	ft_clean_up(t_global **proc)
{
	int			i;
	t_global	*global;

	global = *proc;
	while (global->cmd)
	{
		if (global->cmd->args)
			ft_clear_char_array(&(global->cmd->args),
				global->cmd->args_size + 1);
		if (global->cmd->command)
			free(global->cmd->command);
		if (global->cmd->path)
			free(global->cmd->path);
		if (global->cmd->next)
		{
			global->cmd = global->cmd->next;
			free(global->cmd->prev);
		}
		else if (global->cmd)
			free(global->cmd);
	}
	// clean_up of token array needed
	free(global);
}

void	ft_error(t_global ***proc, char **string)
{
	perror(strerror(errno));
	if (**proc)
		ft_clean_up(*proc);
	if (string)
		free(string);
	exit(EXIT_FAILURE);
}
