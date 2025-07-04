/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 15:52:28 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(int ac, char **av, char **env);
void	minishell_interactive(t_global *global);

int	main(int ac, char **av, char **env)
{
	t_global	global;

	(void)ac;
	(void)av;
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
			ft_process(global);
		free_global(global, false);
	}
}
