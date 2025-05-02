/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 19:08:40 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 22:18:29 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool			ft_is_our_builtin(t_command *cmd, t_global *global);
char			*resolve_command_path(t_global *g, char *path, char *cmd);
static size_t	ft_count_words(char const *s, char c);
static void		ft_check_candidates(char ***i, char **res, char *ex);
void			ft_execute_child_proc(t_command *cmd, t_global *global);

bool	ft_is_our_builtin(t_command *cmd, t_global *global)
{
	(void)global;
	if (ft_strncmp(ECHO, cmd->command, ft_strlen(ECHO)) == 0)
		return (true);
	if (ft_strncmp(CD, cmd->command, ft_strlen(CD)) == 0)
		return (true);
	if (ft_strncmp(EXIT, cmd->command, ft_strlen(EXIT)) == 0)
		return (true);
	if (ft_strncmp(PWD, cmd->command, ft_strlen(PWD)) == 0)
		return (true);
	if (ft_strncmp(EXPORT, cmd->command, ft_strlen(EXPORT)) == 0)
		return (true);
	if (ft_strncmp(UNSET, cmd->command, ft_strlen(UNSET)) == 0)
		return (true);
	if (ft_strncmp(ENV, cmd->command, ft_strlen(ENV)) == 0)
		return (true);
	if (cmd->status_request)
		return (true);
	return (false);
}

char	*resolve_command_path(t_global *g, char *path, char *cmd)
{
	char	*res;
	char	**candidates;
	char	**i;
	char	*ex;

	res = NULL;
	ex = NULL;
	(void)g;
	candidates = ft_split(path, ':');
	if (candidates)
	{
		i = candidates;
		ex = ft_strjoin("/", cmd);
		ft_check_candidates(&i, &res, ex);
	}
	if (ex)
		free_ptr((void **)&ex);
	ft_clear_char_array(&candidates, ft_count_words(path, ':') + 1);
	if (!res)
		res = ft_strdup(cmd);
	return (res);
}

// TODO @aktyz this should be inside libft
static size_t	ft_count_words(char const *s, char c)
{
	size_t	nb_words;
	size_t	i;

	nb_words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			nb_words++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (nb_words);
}

static void	ft_check_candidates(char ***i, char **res, char *ex)
{
	char	**candidates;

	candidates = *i;
	while (*candidates)
	{
		*res = ft_strjoin(*candidates, ex);
		if (access(*res, X_OK) == -1)
		{
			free_ptr((void **) res);
			candidates++;
		}
		else
			break ;
	}
}

void	ft_execute_child_proc(t_command *cmd, t_global *global)
{
	if (cmd->prev && cmd->prev->cmd_pid)
		waitpid(cmd->prev->cmd_pid, NULL, 0);
	if (cmd->is_builtin && !cmd->status_request)
	{
		global->last_exit_code = ft_run_builtin(cmd, global);
		ft_exit(global, cmd->command, global->last_exit_code);
	}
	else
	{
		execve(cmd->path, cmd->args, ft_execve_env(global->env));
		ft_exit(global, cmd->command, EXIT_FAILURE);
	}
}
