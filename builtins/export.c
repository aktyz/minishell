/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:33:45 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 21:26:05 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_export(t_command *cmd, t_global *global);
char		**ft_execve_env(t_list *env);
static void	ft_sort_export_list(t_list **list);
void		ft_handle_existing_var(char *cmd, t_minishell_env *content);
static void	ft_swap_nodes(t_list **current, t_list **next, bool *swapped);

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
		ft_handle_export(cmd, global);
	}
}

/**
 * Function translates our minishell t_list *env into an array required
 * by execve()
 *
 */
char	**ft_execve_env(t_list *env)
{
	char			**execve_env;
	t_minishell_env	*content;
	int				j;

	execve_env = ft_calloc(sizeof(char *), env->lst_size + 1);
	if (!execve_env)
		return (NULL);
	j = 0;
	while (env && env->content)
	{
		content = (t_minishell_env *)env->content;
		if (content->export && content->name_value)
			ft_create_execve_array_entry(&execve_env[j], content);
		if (execve_env[j])
			j++;
		env = env->next;
	}
	return (execve_env);
}

static void	ft_sort_export_list(t_list **list)
{
	t_list			*current;
	t_list			*next;
	bool			swapped;

	if (!list || !*list)
		return ;
	swapped = true;
	while (swapped)
	{
		swapped = false;
		current = *list;
		while (current->next)
		{
			next = current->next;
			if (ft_strcmp(((t_minishell_env *)current->content)->name_value[0],
					((t_minishell_env *)next->content)->name_value[0]) > 0)
				ft_swap_nodes(&current, &next, &swapped);
			current = current->next;
		}
	}
}

void	ft_handle_existing_var(char *cmd, t_minishell_env *content)
{
	int	equal_pos;

	equal_pos = ft_strlen(content->name_value[0]);
	if (equal_pos < (int)ft_strlen(cmd) && cmd[equal_pos] == '=')
	{
		free_ptr((void **)&content->name_value[0]);
		free_ptr((void **)&content->name_value[1]);
		ft_split_env_variable(cmd, &content->name_value[0],
			&content->name_value[1]);
	}
	content->export = true;
}

static void	ft_swap_nodes(t_list **current, t_list **next, bool *swapped)
{
	t_minishell_env	*temp_content;

	temp_content = (*current)->content;
	(*current)->content = (*next)->content;
	(*next)->content = temp_content;
	*swapped = true;
}
