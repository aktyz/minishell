/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 20:42:18 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(int ac, char **av, char **env);
void	minishell_interactive(t_global *global);

int	main(int ac, char **av, char **env)
{
	t_global	global;

	if (ac == 2 && ft_strncmp(av[1], "testing", 8) == 0)
		run_tests(env);
	else
	{
		if (!init_global(&global, env))
			ft_exit(NULL, EXIT_FAILURE);
		minishell_interactive(&global);
	}
	return (0);
}

void	minishell_interactive(t_global *global)
{
	while (1)
	{
		set_signals_interactive();
		global->user_input = readline(PROMPT);
		set_signals_noninteractive();
		if (parse_user_input(global))
		{
			//print_token_list(&global->token);
			//print_cmd_list(global);
			ft_process(global);
		}
		free_global(global, false);
		global->token = NULL;
	}
}
