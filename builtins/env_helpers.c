/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:31:59 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 18:39:11 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_env_var_value(t_list *env, char *var_name);
bool	ft_update_env_var_value(t_list *node, char *new_value);
bool	ft_add_env_var(t_global *g, char *name, char *value);
t_list	*ft_return_env_list_node_ptr(t_list *env, char *name);

char	*ft_get_env_var_value(t_list *env, char *var_name)
{
	t_list			*lst;
	t_minishell_env	*content;

	lst = NULL;
	content = NULL;
	if (!var_name)
		return (NULL);
	lst = ft_return_env_list_node_ptr(env, var_name);
	if (lst)
		content = (t_minishell_env *) lst->content;
	else
		return (NULL);
	if (lst->content && content->name_value[1])
		return (ft_strdup(content->name_value[1]));
	return (NULL);
}

/**
 * Please use this function only when you are sure that the node
 * is pointing to the node of the variable's value you want to update.
 * Because of the overwriting new_value can be empty.
 * Function returns:
 * - true - if the value is successfully update
 * - false - in case of error
 *
 * If new_value is passed as null, the environment variable you
 * want to update will not hold any value (will have `NULL`)
 * under `content->name_value[1]`, therefore we need to check for
 * it in crucial places in the project.
 *
 */
bool	ft_update_env_var_value(t_list *node, char *new_value)
{
	t_minishell_env	*content;
	char			*prev_value;

	prev_value = NULL;
	if (!node)
		return (false);
	content = (t_minishell_env *) node->content;
	if (content->name_value[1]) // The variable already has a value
	{
		prev_value = ft_strdup(content->name_value[1]);
		free_ptr((void **) &content->name_value[1]);
		if (new_value)
		{
			content->name_value[1] = ft_strdup(new_value);
			if (!content->name_value[1])
				return (free_ptr((void **) &prev_value), false);
		}
		else
			content->name_value[1] = NULL;
		return (free_ptr((void **) &prev_value), true);
	}
	else // The variable previously had no value
	{
		if (new_value)
		{
			content->name_value[1] = ft_strdup(new_value);
			if (!content->name_value[1])
				return (false);
		}
		return (true);
	}
}

/**
 * Function creates a new entry on the environment list.
 *
 * We assume that the env list is already there after the initialization
 * process handled by special designation functions.
 *
 * Function takes parameters:
 * - name - that should had been previously validated
 * - value - that can be NULL
 *
 * Because we only create new env variables through `export` cmd,
 * export flag set to true by default.
 */
bool	ft_add_env_var(t_global *g, char *name, char *value)
{
	t_minishell_env	*new_content;

	if (!g->env)
	{
		ft_minishell_perror("ft_add_env_var", 1);
		ft_exit(g, "env_list not initialized", EXIT_FAILURE);
	}
	new_content = ft_calloc(sizeof(t_minishell_env), 1);
	if (!new_content)
	{
		ft_minishell_perror("ft_calloc", 1);
		return (false);
	}
	new_content->name_value = ft_calloc(sizeof(char *), 2);
	if (!new_content->name_value)
	{
		free_ptr((void **) &new_content);
		ft_minishell_perror("ft_calloc", 1);
		return (false);
	}
	new_content->name_value[0] = ft_strdup(name);
	if (value)
		new_content->name_value[1] = ft_strdup(value);
	new_content->export = true;
	ft_lstadd_back(&g->env, ft_lstnew(new_content));
	return (true);
}

/**
 * Function returns a pointer to the env_list node that holds
 * the information for name enviroment variable or NULL if the
 * - list doesn't exist
 * - name is NULL
 * - name is Unfound
 *
 */
t_list	*ft_return_env_list_node_ptr(t_list *env, char *name)
{
	t_list			*lst;
	t_minishell_env	*content;

	if (!env)
		return (NULL);
	if (!name)
		return (NULL);
	lst = env;
	while (lst && lst->content)
	{
		content = (t_minishell_env *) lst->content;
		if (ft_strcmp(name, content->name_value[0]) == 0)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}
