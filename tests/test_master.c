/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:44:34 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/06 16:25:19 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	test_builtins();
static void test_exe_functions(char **env);

void	run_tests(char **env)
{
	test_builtins();
	test_exe_functions(env);
}

static void	test_builtins()
{
	ft_printf("\n======= Testing minishell builtins =======\n");
	test_ft_echo();
}

static void test_exe_functions(char **env)
{
	ft_printf("\n======= Testing minishell builtins =======\n");
	test_extract_env_var(env);
	test_resolve_command_path(env);
}
