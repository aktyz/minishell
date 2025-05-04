/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_execve_arr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:20:30 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 20:49:30 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		**ft_execve_env(t_list *env);
static void	ft_create_execve_array_entry(char **ptr,
				t_minishell_env *content);
bool		process_env_variable(char *env_var, t_list **list);

/**
 * Function translates our minishell t_list *env into an array required
 * by execve(). As our execve is working perfectly fine - DO NOT TOUCH
 *
 */
char	**ft_execve_env(t_list *env)
{
	char			**execve_env;
	t_minishell_env	*content;
	int				i;

	execve_env = ft_calloc(sizeof(char *), env->lst_size + 1);
	if (!execve_env)
		return (NULL);
	i = 0;
	while (env && env->content)
	{
		content = (t_minishell_env *)env->content;
		if (content->export && content->name_value)
			ft_create_execve_array_entry(&execve_env[i], content);
		if (execve_env[i])
			i++;
		env = env->next;
	}
	return (execve_env);
}

static void	ft_create_execve_array_entry(char **ptr, t_minishell_env *content)
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

/**
 * This is a special function handling env variables list creation
 * in the initialization phase of the minishell start-up.
 * aka: DO NOT TOUCH
 */
bool	process_env_variable(char *env_var, t_list **list)
{
	t_minishell_env	*content;

	content = ft_calloc(sizeof(t_minishell_env), 1);
	if (!content)
	{
		ft_lstclear(list, ft_clear_minishell_env);
		return (false);
	}
	content->name_value = ft_calloc(sizeof(char *), 2);
	if (!content->name_value)
	{
		ft_lstclear(list, ft_clear_minishell_env);
		return (false);
	}
	ft_split_env_variable(env_var, &content->name_value[0],
		&content->name_value[1]);
	if (!content->name_value[0] || !content->name_value[1])
	{
		free(content);
		ft_lstclear(list, ft_clear_minishell_env);
		return (false);
	}
	if (ft_strcmp(content->name_value[0], "_"))
		content->export = true;
	return (ft_lstadd_back(list, ft_lstnew(content)), true);
}
