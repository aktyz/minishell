/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/09 17:00:11 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		print_tokens(t_token *list);
void		minishell_interactive(t_global *global);




// NOTE: comment this function to run test cases from tests.c
// criterion test runner adds its own main function

/**/
int	main(int ac, char **av, char **env)
{
	t_global	global;

	if (ac == 2 && ft_strncmp(av[1], "testing", 8) == 0)
		run_tests(env);
	else
	{
		if (!init_global(&global, env))
			exit_shell(NULL, EXIT_FAILURE);
		minishell_interactive(&global);
	}
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
		// set_signals_interactive();
		global->user_input = readline(PROMPT);
		// set_signals_noninteractive();
		if (parse_user_input(global))
		{
			//print_token_list(&global->token);
			print_cmd_list(global);
		}
		//g_last_exit_code = execute(global);
		//else
		//	g_last_exit_code = 1;
		ft_process(global);
		free_global(global, false);
		global->token = NULL;
	}
}
