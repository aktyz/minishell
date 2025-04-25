/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:07:42 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/25 20:49:33 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	add_detail_quotes(char *command);
int			errmsg_cmd(char *command, char *detail, char *error_message,
				int error_nb);
void		errmsg(char *errmsg, char *detail, int quotes);

/* add_detail_quotes:
 *	Checks whether to add quotes around the error detail:
 *	i.e. "unset: `@': not a valid identifier"
 *	Returns true if the command is export or unset,
 *	false if not.
 */
static bool	add_detail_quotes(char *command)
{
	if (ft_strncmp(command, "export", 7) == 0 || ft_strncmp(command, "unset",
			6) == 0)
		return (true);
	return (false);
}

/* errmsg_cmd:
 *	Prints an error message to the standard error, prefixed with the
 *	program name.
 *	Returns with the specified error number.
 */
int	errmsg_cmd(char *command, char *detail, char *error_message, int error_nb)
{
	char	*msg;
	bool	detail_quotes;

	detail_quotes = add_detail_quotes(command);
	msg = ft_strdup(MINISHELL);
	if (command != NULL)
	{
		msg = ft_strjoin(msg, command);
		msg = ft_strjoin(msg, ": ");
	}
	if (detail != NULL)
	{
		if (detail_quotes)
			msg = ft_strjoin(msg, "'");
		msg = ft_strjoin(msg, detail);
		if (detail_quotes)
			msg = ft_strjoin(msg, "'");
		msg = ft_strjoin(msg, ": ");
	}
	msg = ft_strjoin(msg, error_message);
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
	return (error_nb);
}

/* errmsg:
 *	Prints an error message that is unrelated to a specific command.
 *	Used in parsing phase for syntax errors.
 */
void	errmsg(char *errmsg, char *detail, int quotes)
{
	char	*msg;

	msg = ft_strdup(MINISHELL);
	msg = ft_strjoin(msg, errmsg);
	if (quotes)
		msg = ft_strjoin(msg, " '");
	else
		msg = ft_strjoin(msg, ": ");
	msg = ft_strjoin(msg, detail);
	if (quotes)
		msg = ft_strjoin(msg, "'");
	ft_putendl_fd(msg, STDERR_FILENO);
	free_ptr(msg);
}
