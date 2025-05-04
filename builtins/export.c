/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:33:45 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 20:40:44 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_export(t_command *cmd, t_global *global);
static int	ft_handle_export_arg(char *cmd, t_global *global);
static void	ft_handle_export_no_arg(t_global *global);
void		ft_mini_export_wrapper(t_command *cmd, t_global *global);

void	ft_export(t_command *cmd, t_global *global)
{
	t_list	*env;
	int		i;

	env = global->env;
	i = 0;
	if (cmd->args[1])
	{
		while (cmd->args[++i])
			global->last_exit_code = ft_handle_export_arg(cmd->args[i], global);
	}
	else
	{
		ft_sort_export_list(&env);
		ft_handle_export_no_arg(global);
	}
}

/**
 * This function is running whenever user provides argument to
 * export built-in command. It needs to:
 * - create variable if not there
 * - update variable value if variable is there
 * - in all cases flip env->export to true
 *
 */
static int	ft_handle_export_arg(char *cmd, t_global *global)
{
	char	*value;
	char	*final;
	char	*tmp;

	value = ft_strjoin("minishell: ", cmd);
	final = ft_strjoin(*ft_split(value, '='), ": not a valid identifier\n");
	tmp = ft_strdup(cmd);
	if (!is_valid_var_name(tmp))
	{
		ft_putstr_fd(final, 2);
		free_ptr((void **) &value);
		free_ptr((void **) &final);
		free_ptr((void **) &tmp);
		return (1);
	}
	ft_update_value_or_add(cmd, global);
	free_ptr((void **) &final);
	free_ptr((void **) &tmp);
	return (free_ptr((void **) &value), 0);
}

static void	ft_handle_export_no_arg(t_global *global)
{
	t_minishell_env	*content;
	t_list			*env;

	env = global->env;
	while (env && env->content)
	{
		content = (t_minishell_env *)env->content;
		if (content->export && content->name_value)
		{
			if (!content->name_value[1])
				ft_printf("declare -x %s\n", content->name_value[0]);
			else
				ft_printf("declare -x %s=\"%s\"\n", content->name_value[0],
					content->name_value[1]);
		}
		env = env->next;
	}
}

void	ft_mini_export_wrapper(t_command *cmd, t_global *global)
{
	t_io_fds	*io;

	io = NULL;
	if (cmd->final_io)
		io = cmd->final_io;
	if ((global->cmd->content != cmd) || (io && io->infile))
	{
		ft_clear_char_array(&cmd->args, cmd->args_size);
		cmd->args = ft_calloc(sizeof(char *), 2);
		cmd->args[0] = ft_strdup(EXPORT);
	}
	ft_export(cmd, global);
}
