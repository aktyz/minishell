/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:33:45 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 17:38:27 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_export(t_command *cmd, t_global *global);
static bool	ft_handle_export_arg(char *cmd, t_global *global);
static void	ft_handle_export_no_arg(t_global *global);
void		ft_mini_export_wrapper(t_command *cmd, t_global *global);

void	ft_export(t_command *cmd, t_global *global)
{
	t_list	*env;
	bool	result;
	int		i;

	env = global->env;
	i = 0;
	if (cmd->args[1])
	{
		while (cmd->args[++i])
		{
			result = ft_handle_export_arg(cmd->args[i], global);
			if (!result)
				global->last_exit_code = 1;
		}
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
 * - validate var_name, if ok it:
 *     - create variable if not there
 *     - update variable value if variable is there
 * -> in all cases flip env->export to true
 *
 * It takes export_arg argument - passed from export of structure:
 * <var_name>=<var_value> OR <var_name>
 */
static bool	ft_handle_export_arg(char *export_arg, t_global *global)
{
	char	*var_name;
	char	*var_value;
	t_list	*node;
	bool	result;

	var_value = NULL;
	node = NULL;
	result = false;
	if (ft_strnchar(export_arg, '='))
		ft_split_env_variable(export_arg, &var_name, &var_value);
	else
		var_name = ft_strdup(export_arg);
	node = ft_return_env_list_node_ptr(global->env, var_name);
	if (is_valid_var_name(var_name))
	{
		if (node)
			result = ft_update_env_var_value(node, var_value);
		else
			result = ft_add_env_var(global, var_name, var_value);
	}
	else
		errmsg_cmd("export", NULL, "not a valid identifier", 1);
	if (var_value)
		free_ptr((void **)&var_value);
	return (free_ptr((void **)&var_name), result);
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
