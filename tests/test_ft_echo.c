/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_ft_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:32:31 by zslowian          #+#    #+#             */
/*   Updated: 2025/03/26 17:02:48 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**multiple_ens(void);
static char	**no_ens(void);
static char	**no_arg(void);
static char	**no_str(void);

void	test_ft_echo(void)
{
	char	**args;

	args = multiple_ens();
	ft_echo(args);
	ft_clear_char_array(&args, 7);
	args = no_ens();
	ft_echo(args);
	ft_clear_char_array(&args, 5);
	args = no_arg();
	ft_echo(args);
	ft_clear_char_array(&args, 3);
	args = no_str();
	ft_echo(args);
	ft_clear_char_array(&args, 2);
}

static char	**multiple_ens(void)
{
	char	**args;

	ft_printf("Testing: [echo -n -n -n Hello World!]\n");
	args = (char **)malloc(sizeof(char *) * 7);
	args[0] = ft_strdup("echo");
	args[1] = ft_strdup("-n");
	args[2] = ft_strdup("-n");
	args[3] = ft_strdup("-n");
	args[4] = ft_strdup("Hello");
	args[5] = ft_strdup("World!");
	args[6] = NULL;
	return (args);
}

static char	**no_ens(void)
{
	char	**args;

	ft_printf("Testing: [echo Hello World! -n]\n");
	args = (char **)malloc(sizeof(char *) * 5);
	args[0] = ft_strdup("echo");
	args[1] = ft_strdup("Hello");
	args[2] = ft_strdup("World!");
	args[3] = ft_strdup("-n");
	args[4] = NULL;
	return (args);
}

static char	**no_arg(void)
{
	char	**args;

	ft_printf("Testing: [echo -n]\n");
	args = (char **)malloc(sizeof(char *) * 3);
	args[0] = ft_strdup("echo");
	args[1] = ft_strdup("-n");
	args[2] = NULL;
	return (args);
}

static char	**no_str(void)
{
	char **args;

	ft_printf("Testing: [echo]\n");
	args = (char **)malloc(sizeof(char *) * 2);
	args[0] = ft_strdup("echo");
	args[2] = NULL;
	return (args);
}