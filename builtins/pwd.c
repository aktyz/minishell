/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:36:36 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 13:46:51 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*buff;

	buff = ft_getcwd();
	if (!buff)
		return (1);
	ft_printf("%s\n", buff);
	free(buff);
	return (0);
}

char	*ft_getcwd(void)
{
	char	*buff;
	int		buff_size;

	buff_size = 42;
	buff = ft_calloc(sizeof(char), buff_size);
	while (getcwd(buff, buff_size) == NULL)
	{
		free(buff);
		buff_size *= 2;
		buff = ft_calloc(sizeof(char), buff_size);
	}
	return (buff);
}
