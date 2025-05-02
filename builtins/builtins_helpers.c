/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:45:11 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 23:08:36 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_var_name(char *var_name);
void	ft_update_value_or_add(char *cmd, t_global *global);
void	ft_mini_export_wrapper(t_command *cmd, t_global *global);

bool	is_valid_var_name(char *var_name)
{
	int		i;
	char	**name_value;
	char	*tmp;

	if (!var_name || !var_name[0] || var_name[0] == '=')
		return (false);
	i = 0;
	name_value = NULL;
	tmp = ft_strdup(var_name);
	if (ft_strchr(tmp, '='))
		name_value = ft_split(tmp, '=');
	while (name_value[0] && name_value[0][i])
	{
		if (!(ft_isalpha(name_value[0][i])) || !(ft_isdigit(name_value[0][i]))
				|| !(name_value[0][i] == '_'))
				{
					free_ptr((void **) &tmp);
					ft_clear_char_array(&name_value, 2);
					return (true);
				}
		i++;
	}
	free_ptr((void **) &tmp);
	ft_clear_char_array(&name_value, 2);
	return (false);
}

void	ft_update_value_or_add(char *cmd, t_global *global)
{
	t_minishell_env	*content;
	t_list			*env;
	char			*split_ptr;
	int				split_index;

	env = global->env;
	while (env && env->content)
	{
		content = (t_minishell_env *)env->content;
		split_ptr = ft_strchr(cmd, '=');
		split_index = split_ptr - cmd;
		if (ft_strncmp((const char *)cmd, content->name_value[0],
				split_index) == 0)
		{
			ft_handle_existing_var(cmd, content);
			return ;
		}
		env = env->next;
	}
	if (!env)
		ft_add_new_env_var(cmd, global);
}

void	ft_mini_export_wrapper(t_command *cmd, t_global *global)
{
	if (cmd->pipe_output || (cmd->io_fds && cmd->io_fds->outfile))
		return ;
	if ((cmd->prev && cmd->prev->pipe_output)
		|| (cmd->io_fds && cmd->io_fds->infile))
	{
		ft_clear_char_array(&cmd->args, cmd->args_size);
		cmd->args = ft_calloc(sizeof(char *), 2);
		cmd->args[0] = ft_strdup(EXPORT);
	}
	ft_export(cmd, global);
}
