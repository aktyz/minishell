/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_master.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:44:34 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/23 17:54:24 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	test_builtins(void);
static void	test_exe_functions(char **env);

void	run_tests(char **env)
{
	test_builtins();
	test_exe_functions(env);
}

static void	test_builtins(void)
{
	ft_printf("\n======= Testing minishell builtins =======\n");
	test_ft_echo();
}

static void	test_exe_functions(char **env)
{
	ft_printf("\n======= Testing minishell builtins =======\n");
}
