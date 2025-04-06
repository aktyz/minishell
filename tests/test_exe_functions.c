/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exe_functions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:17:51 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/06 16:43:21 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	test_extract_env_var(char **env)
{
	char	*path;
	char	*cmd;

	path = extract_env_var(ENV_PATH, env);
	ft_printf("%s\n", path);
	cmd = extract_env_var("_", env);
	ft_printf("%s\n", cmd);
	path = extract_env_var("blahnotfound", env);
	if (path != NULL)
		ft_printf("Extracting unexisting variable failed!\n");
}

void	test_resolve_command_path(char **env)
{
	char	*path;

	path = resolve_command_path(extract_env_var(ENV_PATH, env), "ls");
	ft_printf("\n%s\n\n", path);
}
