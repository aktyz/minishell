/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:22 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 18:18:08 by zslowian         ###   ########.fr       */
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
			ft_printf("%s=%s\n", content->name_value[0], content->name_value[1]);
		ptr = ptr->next;
	}
	return (0);
}
