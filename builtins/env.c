/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:22 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 22:27:58 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_list *env)
{
	t_list			*ptr;
	t_minishell_env	*content;

	ptr = env;
	while (ptr && ptr->content)
	{
		content = (t_minishell_env *)ptr->content;
		ft_printf("%s=%s\n", content->name_value[0], content->name_value[1]);
		ptr = ptr->next;
	}
	return (0);
}
