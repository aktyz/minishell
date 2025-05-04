/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:15:55 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 20:39:29 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_var_name(char *var_name);
void	ft_split_env_variable(char *name_value, char **var_name,
			char **var_value);
void	ft_handle_existing_var(char *cmd, t_minishell_env *content);
void	ft_update_value_or_add(char *cmd, t_global *global);

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

void	ft_split_env_variable(char *name_value, char **var_name,
			char **var_value)
{
	char	*split_char;
	int		split_index;
	int		total_length;

	split_char = NULL;
	split_char = ft_strchr(name_value, '=');
	if (split_char)
	{
		split_index = split_char - name_value;
		total_length = ft_strlen(name_value);
		*var_name = ft_substr(name_value, 0, split_index);
		*var_value = ft_substr(name_value, split_index + 1, total_length
				- split_index - 1);
	}
	else
		*var_name = ft_strdup(name_value);
}

void	ft_handle_existing_var(char *cmd, t_minishell_env *content)
{
	int	equal_pos;

	equal_pos = ft_strlen(content->name_value[0]);
	if (equal_pos < (int) ft_strlen(cmd) && cmd[equal_pos] == '=')
	{
		free_ptr((void **)&content->name_value[0]);
		free_ptr((void **)&content->name_value[1]);
		ft_split_env_variable(cmd, &content->name_value[0],
			&content->name_value[1]);
	}
	content->export = true;
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
