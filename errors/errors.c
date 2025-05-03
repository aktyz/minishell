/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:07:42 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/03 15:56:09 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_minishell_perror(char *cmd, int status);
static bool	add_detail_quotes(char *command);
int			errmsg_cmd(char *command, char *detail, char *error_message,
				int error_nb);

void	ft_minishell_perror(char *cmd, int status)
{
	char	*s;

	s = ft_strjoin("minishell: ", cmd);
	errno = status;
	perror(s);
	free_ptr((void **) &s);
}

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

	if (command)
		detail_quotes = add_detail_quotes(command);
	msg = ft_strdup("minishell: ");
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
	return (free_ptr((void **) &msg), error_nb);
}
