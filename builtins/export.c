/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:33:45 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 18:21:16 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		**ft_execve_env(t_list *env);
static void	ft_sort_export_list(t_list **list);
static void	ft_handle_existing_var(t_command *cmd, t_minishell_env *content);
void		ft_split_env_variable(char *name_value, char **var_name,
				char **var_value);

void	ft_export(t_command *cmd, t_global *global)
{
	t_list			*env;
	t_minishell_env	*content;
	char			*var_name;

	env = global->env;
	if (cmd->args[1])
	{
		var_name = cmd->args[1];
		while (env && env->content)
		{
			content = (t_minishell_env *)env->content;
			if (ft_strcmp(var_name, content->name_value[0]) == 0)
				// var already exists
				return (ft_handle_existing_var(cmd, content));
			env = env->next;
		}
		if (!env) // var doesn't exist -> add it
		{
			content = ft_calloc(sizeof(t_minishell_env), 1);
			if (!content)
				return ;
			content->name_value = ft_calloc(sizeof(char *), 2);
			ft_split_env_variable(var_name, &content->name_value[0],
				&content->name_value[1]);
			content->export = true;
			ft_lstadd_back(&global->env, ft_lstnew(content));
		}
		return ;
	}
	else // export with no args or parameters
	{
		ft_sort_export_list(&env);
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
} // Norm: Function too long

/**
 * Function translates our minishell t_list *env into an array required
 * by execve()
 *
 */
char	**ft_execve_env(t_list *env)
{
	char			**execve_env;
	t_minishell_env	*content;
	char			*env_var;
	int				i;
	int				j;

	execve_env = ft_calloc(sizeof(char *), env->lst_size + 1);
	if (!execve_env)
		return (NULL);
	j = 0;
	while (env && env->content)
	{
		content = (t_minishell_env *)env->content;
		if (content->export && content->name_value)
		{
			env_var = ft_calloc(sizeof(char), ft_strlen(content->name_value[0])
					+ ft_strlen(content->name_value[1]) + 2);
			i = ft_strlcpy(env_var, content->name_value[0],
					ft_strlen(content->name_value[0]));
			env_var[i] = '=';
			ft_strlcpy(env_var + i + 1, content->name_value[1],
				ft_strlen(content->name_value[1]));
			execve_env[j] = env_var;
		}
		if (execve_env[j])
			j++;
		env = env->next;
	}
	return (execve_env);
} // Norm: Function too long

static void	ft_sort_export_list(t_list **list)
{
	t_list			*current;
	t_list			*next;
	t_minishell_env	*temp_content;
	int				swapped;

	if (!list || !*list)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = *list;
		while (current->next)
		{
			next = current->next;
			if (ft_strcmp(((t_minishell_env *)current->content)->name_value[0],
					((t_minishell_env *)next->content)->name_value[0]) > 0)
			{
				temp_content = current->content;
				current->content = next->content;
				next->content = temp_content;
				swapped = 1;
			}
			current = current->next;
		}
	}
} // Norm: Function too long

static void	ft_handle_existing_var(t_command *cmd, t_minishell_env *content)
{
	int	equal_pos;

	equal_pos = ft_strlen(content->name_value[0]);
	if (cmd->args[1][equal_pos] == '=')
	{
		free_ptr(content->name_value[0]);
		free_ptr(content->name_value[1]);
		ft_split_env_variable(cmd->args[1], &content->name_value[0],
			&content->name_value[1]);
	}
	content->export = true;
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
