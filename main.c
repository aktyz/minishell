/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 19:13:21 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		main(int ac, char **av, char **env);
void	minishell_interactive(t_global *global);

int	main(int ac, char **av, char **env)
{
	t_global	global;

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
			print_cmd_list(global);
			clean_unnecessary_fds(global);
			ft_process(global);
		} else
			global->last_exit_code = 1;
		free_global(global, false);
	}
}
