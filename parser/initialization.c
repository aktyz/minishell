/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:04:07 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 21:30:48 by mwiecek          ###   ########.fr       */
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
	t_minishell_env	*content;
	int				i;

	(void)content;
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
		ft_exit(global, "Fatal", EXIT_FAILURE);
	global->token = NULL;
	global->user_input = NULL;
	global->is_global = true;
	global->cmd = NULL;
	global->last_exit_code = 0;
	return (true);
}

void	init_io(t_command *cmd)
{
	if (!cmd->io_fds)
	{
		cmd->io_fds = malloc(sizeof * cmd->io_fds);
		if (!cmd->io_fds)
			return ;
		cmd->io_fds->infile = NULL;
		cmd->io_fds->outfile = NULL;
		cmd->io_fds->heredoc_delimiter = NULL;
		cmd->io_fds->heredoc_quotes = false;
		cmd->io_fds->fd_in = -1;
		cmd->io_fds->fd_out = -1;
		cmd->io_fds->stdin_backup = -1;
		cmd->io_fds->stdout_backup = -1;
	}
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
