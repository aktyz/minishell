/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:18:25 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/09 17:01:21 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *single_command_test_cases(int i);

/**
 * This function is considered a test function,
 * mocking an execution of "grep -la" bash command
 *
 */
void	test_single_cmd(t_global *global)
{
	t_command	*cmd;
	int			i;
	int			nb_test_cases;

	cmd = global->cmd;
	nb_test_cases = 2;
	i = 0;
	while (i < nb_test_cases)
	{
		ft_get_path_and_args(&cmd, single_command_test_cases(i), NULL);
		if (i < nb_test_cases - 1)
		{
			cmd->next = ft_calloc(1, sizeof(t_command));
			cmd->next->prev = cmd;
			cmd = cmd->next;
		}
		i++;
	}
	cmd = global->cmd;
	i = 0;
	while (i < nb_test_cases)
	{
		cmd->path = resolve_command_path(extract_env_var(ENV_PATH, global->env), cmd->args[0]);
		cmd = cmd->next;
		i++;
	}
	// TODO: execute ft_process here on those commands to complete the tests
}

static char *single_command_test_cases(int i)
{
	char	*table[2];

	table[0] = "ls -la";
	table[1] = "grep -r '\"minishell\"";
	//table[3] = "test case with variable inside" + increase `nb_test_cases` in the above function
	return (table[i]);
}
