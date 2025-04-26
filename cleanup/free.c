/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:56:12 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 17:40:13 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Save free the pointer. The NULL exuation need to be up in the function
 * otherwise it won't work.
 *
*/
void	free_ptr(void **ptr)
{
	if (ptr && *ptr)
		free(*ptr);
	*ptr = NULL;
}

/* free_io:
*	Frees the input/output fd structure.
*/
void	free_io(t_io_fds *io)
{
	if (!io)
		return ;
	restore_io(io);
	if (io->heredoc_delimiter)
	{
		unlink(io->infile);
		free_ptr((void **)&io->heredoc_delimiter);
	}
	if (io->infile)
		free_ptr((void **)&io->infile);
	if (io->outfile)
		free_ptr((void **)&io->outfile);
	if (io)
		free_ptr((void **)&io);
}

/* free_str_tab:
*	Frees an array of strings.
*/
void	free_str_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
			{
				free_ptr((void **)&tab[i]);
				tab[i] = NULL;
			}
			i++;
		}
		free(tab);
		tab = NULL;
	}
}

/* free_data:
*	Frees all of the data used to run a command. If clear_history is true,
*	frees the environment and the command history before returning.
*/
void	free_global(t_global *global, bool clear_history)
{
	if (global && global->user_input)
		free_ptr((void **)&global->user_input);
	if (global && global->cmd)
		lst_clear_cmd(&global->cmd, &free_ptr);
	if (global && global->token)
		ft_clear_token(&global->token);
}
