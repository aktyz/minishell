/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_append_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:58:27 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/02 21:08:55 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	***APPEND***
	Redirection of output in append mode causes the file whose name results
	from the expansion of word to be opened for appending on file descriptor n,
	or the standard output (fd 1) if n is not specified.
	If the file does not exist it is created.

	The general format for appending output is: [n]>>word.

*/

/* open_outfile_append:
*	Opens an outfile in append mode. If an outfile was already set, frees it
*	and overwrites it. If a previous infile or outfile open failed (file does
*	not exist or permission denied), does not open any further output file.
*
*	Ex.:
*		echo hello > forbidden_file >> test
*		echo hello >> forbidden_file >> test
*		< forbidden_file cat >> test
*	In these 3 cases, the test file should not be opened or created.
*/


void	parse_pipe(t_global *g, t_command **curr_cmd, t_token **curr_token)
{
	(*curr_cmd)->pipe_output = true;
	ft_lstadd_back(&g->cmd, ft_lstnew(lst_new_cmd()));
	*curr_cmd = lst_last_cmd(g->cmd);
	*curr_token = (*curr_token)->next;
}
