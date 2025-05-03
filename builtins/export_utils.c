/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 12:15:55 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 15:54:07 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_split_env_variable(char *name_value, char **var_name,
				char **var_value);
void		ft_create_execve_array_entry(char **ptr, t_minishell_env *content);
int			ft_handle_export_arg(char *cmd, t_global *global);
void		ft_handle_export(t_global *global);
void		ft_add_new_env_var(char *var_name, t_global *global);

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

void	ft_create_execve_array_entry(char **ptr, t_minishell_env *content)
{
	char	*env_var;
	int		i;

	env_var = ft_calloc(sizeof(char), ft_strlen(content->name_value[0])
			+ ft_strlen(content->name_value[1]) + 2);
	i = ft_strlcpy(env_var, content->name_value[0],
			ft_strlen(content->name_value[0]));
	env_var[i] = '=';
	ft_strlcpy(env_var + i + 1, content->name_value[1],
		ft_strlen(content->name_value[1]));
	*ptr = env_var;
}

int	ft_handle_export_arg(char *cmd, t_global *global)
{
	char	*value;
	char	*final;

	value = ft_strjoin("minishell: ", cmd);
	final = ft_strjoin(value, ": not a valid identifier\n");
	if (!is_valid_var_name(cmd))
	{
		ft_putstr_fd(final, 2);
		free_ptr((void **) &value);
		free_ptr((void **) &final);
		return (1);
	}
	ft_update_value_or_add(cmd, global);
	free_ptr((void **) &final);
	return (free_ptr((void **) &value), 0);
}

void	ft_handle_export(t_global *global)
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
				ft_printf("declare -x %s=\"\"\n", content->name_value[0]);
			else
				ft_printf("declare -x %s=\"%s\"\n", content->name_value[0],
					content->name_value[1]);
		}
		env = env->next;
	}
}

void	ft_add_new_env_var(char *var_name, t_global *global)
{
	t_minishell_env	*content;

	content = ft_calloc(sizeof(t_minishell_env), 1);
	if (!content)
		return ;
	content->name_value = ft_calloc(sizeof(char *), 2);
	ft_split_env_variable(var_name, &content->name_value[0],
		&content->name_value[1]);
	content->export = true;
	ft_lstadd_back(&global->env, ft_lstnew(content));
}
