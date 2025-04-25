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
void	free_ptr(void *ptr)
{
	if (ptr != NULL)
		free(ptr);
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
		free_ptr(io->heredoc_delimiter);
	}
	if (io->infile)
		free_ptr(io->infile);
	if (io->outfile)
		free_ptr(io->outfile);
	if (io)
		free_ptr(io);
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
				free_ptr(tab[i]);
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
	{
		free_ptr(global->user_input);
		global->user_input = NULL;
	}
	if (global && global->cmd)
		lst_clear_cmd(&global->cmd, &free_ptr);
	//if (global && global->cmd)
	//	lst_clear_cmd(&global->token, &free_ptr); // TODO @aktyz we need to clean Token list as well here
}
