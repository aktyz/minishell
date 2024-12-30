/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_ft_lst_to_arr.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 16:03:10 by zslowian          #+#    #+#             */
/*   Updated: 2024/12/16 20:13:26 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_lst_to_arr(t_list *argv);
void	ft_delete_lst_node(t_list *node);
void	ft_delete_lst(t_list **node, int size);

char	**ft_lst_to_arr(t_list *argv)
{
	int		size;
	char	**arr;
	char	**tmp;

	size = ft_lstsize(argv) + 1;
	arr = malloc(sizeof(char *) * size);
	if (!arr)
		return (NULL);
	tmp = arr;
	while (size > 1)
	{
		*tmp = ft_calloc(sizeof(char), ft_strlen(argv->content) + 1);
		ft_strlcpy(*tmp, argv->content, ft_strlen(argv->content));
		argv = argv->next;
		tmp++;
		size--;
	}
	*tmp = NULL;
	return (arr);
}

void	ft_delete_lst_node(t_list *node)
{
	if (node->content)
		free(node->content);
	if (node)
		free(node);
}

void	ft_delete_lst(t_list **node, int size)
{
	t_list		*lst;

	while (size > 0)
	{
		lst = (*node)->next;
		ft_delete_lst_node(*node);
		*node = lst;
		size--;
	}
}
