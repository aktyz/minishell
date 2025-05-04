/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:04:07 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 08:28:25 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_var_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

bool	init_env(t_global *global, char **env)
{
	t_list			*list;
	int				i;

	list = NULL;
	i = 0;
	while (env[i])
	{
		if (!process_env_variable(env[i], &list))
		{
			ft_lstclear(&list, ft_clear_minishell_env);
			return (false);
		}
		i++;
	}
	global->env = list;
	return (true);
}

bool	init_global(t_global *global, char **env)
{
	if (!init_env(global, env))
	{
		ft_handle_minishell_err("init_env", "could not initialize minishell\n");
		ft_exit(global, "fatal", EXIT_FAILURE);
	}
	global->token = NULL;
	global->user_input = NULL;
	global->is_global = true;
	global->cmd = NULL;
	global->last_exit_code = 0;
	return (true);
}

char	**ft_trim_user_input(char **argv, int argc)
{
	int		i;
	int		j;
	char	**result;

	i = 1;
	j = 0;
	result = ft_calloc(sizeof(char *), argc - 1);
	while (i < argc)
	{
		result[j] = ft_strtrim(argv[i], TRIM_SET);
		i++;
		j++;
	}
	return (result);
}
