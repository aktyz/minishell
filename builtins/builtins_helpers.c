/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 11:45:11 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 13:07:00 by zslowian         ###   ########.fr       */
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


	if (!var_name || !var_name[0] || var_name[0] == '=' || ft_isdigit(var_name[0]))
		return (false);
	i = 0;
	if (ft_strchr(var_name, '='))
	{
		name_value = ft_split(var_name, '=');
		free_ptr((void **) &var_name);
		var_name = ft_strdup(name_value[0]);
		ft_clear_char_array(&name_value, 2);
	}
	while (var_name[i])
	{
		if (!(ft_isalpha(var_name[i]) || ft_isdigit(var_name[i])
				|| var_name[i] == '_'))
			return (false);
		i++;
	}
	return (true);
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
		
		if (split_index != (int) ft_strlen(content->name_value[0])) {
			env = env->next;
			continue ;
		}
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
	t_io_fds	*io;

	io = NULL;
	if (cmd->final_io)
		io = cmd->final_io;
	if (cmd->pipe_output || (io && io->outfile))
		return ;
	if ((global->cmd->content != cmd) || (io && io->infile))
	{
		ft_clear_char_array(&cmd->args, cmd->args_size);
		cmd->args = ft_calloc(sizeof(char *), 2);
		cmd->args[0] = ft_strdup(EXPORT);
	}
	ft_export(cmd, global);
}
