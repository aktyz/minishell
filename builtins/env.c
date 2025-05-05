/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:22 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 17:43:29 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_list *env)
{
	t_list			*ptr;
	t_minishell_env	*content;

	ptr = env;
	content = (t_minishell_env *)ptr->content;
	while (ptr && ptr->content)
	{
		content = (t_minishell_env *)ptr->content;
		if (content->name_value[1])
			ft_printf("%s=%s\n", content->name_value[0],
				content->name_value[1]);
		ptr = ptr->next;
	}
	return (0);
}

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
		content = (t_minishell_env *)lst->content;
	else
		return (NULL);
	if (lst->content && content->name_value[1])
		return (ft_strdup(content->name_value[1]));
	return (NULL);
}
