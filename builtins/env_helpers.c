/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:31:59 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 17:53:38 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_update_env_var_value(t_list *node, char *new_value);
bool	ft_add_env_var(t_global *g, char *name, char *value);
void	ft_add_new_env_var(char *var_name, t_global *global);
t_list	*ft_return_env_list_node_ptr(t_list *env, char *name);
bool	ft_update_existing_env_value(t_minishell_env **node, char *new_value);

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
	bool			res;

	res = false;
	if (!node)
		return (res);
	content = (t_minishell_env *)node->content;
	if (content->name_value[1])
		res = ft_update_existing_env_value(&content, new_value);
	else
	{
		if (new_value)
		{
			content->name_value[1] = ft_strdup(new_value);
			if (!content->name_value[1])
				return (res);
			res = true;
		}
	}
	content->export = true;
	return (res);
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
	new_content = init_env_content(name, value);
	if (new_content == NULL)
	{
		ft_minishell_perror("ft_add_env_var", 1);
		ft_exit(g, "new env node not initialized", EXIT_FAILURE);
	}
	new_content->export = true;
	ft_lstadd_back(&g->env, ft_lstnew(new_content));
	return (true);
}

/**
 * This function is very similar to the one above, however
 * instead of two arguments env_var_name and env_var_value
 * it takes a string var_name as argument consisting of:
 * <var_name>=<var_value>
 *
 */
void	ft_add_new_env_var(char *var_name, t_global *global)
{
	t_minishell_env	*content;

	content = init_env_content(NULL, NULL);
	ft_split_env_variable(var_name, &content->name_value[0],
		&content->name_value[1]);
	content->export = true;
	ft_lstadd_back(&global->env, ft_lstnew(content));
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
		content = (t_minishell_env *)lst->content;
		if (ft_strcmp(name, content->name_value[0]) == 0)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

bool	ft_update_existing_env_value(t_minishell_env **node, char *new_value)
{
	char	*prev_value;

	prev_value = ft_strdup((*node)->name_value[1]);
	free_ptr((void **) &(*node)->name_value[1]);
	if (new_value)
	{
		(*node)->name_value[1] = ft_strdup(new_value);
		if (!(*node)->name_value[1])
			return (free_ptr((void **) &prev_value), false);
	}
	else
		(*node)->name_value[1] = NULL;
	return (true);
}
