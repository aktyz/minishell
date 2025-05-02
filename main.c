/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 21:20:50 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(int ac, char **av, char **env);
void	minishell_interactive(t_global *global);

int	main(int ac, char **av, char **env)
{
	t_global	global;

	(void)av;
	(void)ac;
	if (init_global(&global, env))
		minishell_interactive(&global);
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
			clean_unnecessary_fds(global);
			ft_process(global);
		}
		free_global(global, false);
	}
}
