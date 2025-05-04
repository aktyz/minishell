/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:22:35 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 22:32:16 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_cd(t_command *cmd, t_global *g);
static int	ft_cd_home(t_global *g, char *cmd);
static void	handle_cd_err(char *cmd, t_global *g, int code);

int	ft_cd(t_command *cmd, t_global *g)
{
	char	*pwd;
	char	*old_pwd;

	pwd = ft_get_env_var_value("PWD", g->env);
	if (cmd->args[2])
	{
		handle_cd_err(cmd->args[0], g, 1);
		return (1);
	}
	if (!cmd->args[1])
	{
		if (ft_cd_home(g, cmd->command) == -1)
			return (1);
	}
	else if (chdir(cmd->args[1]) == -1)
	{
		handle_cd_err(cmd->args[1], g, 2);
		return (1);
	}
	old_pwd = ft_strjoin("OLDPWD=", pwd);
	free_ptr((void **) &pwd);
	if (cmd->args[1])
		pwd = ft_strjoin("PWD=", ft_getcwd());
	else
		pwd = ft_get_env_var_value("HOME", g->env);
	ft_update_value_or_add(old_pwd, g);
	ft_update_value_or_add(pwd, g);
	free_ptr((void **) &pwd);
	free_ptr((void **) &old_pwd);
	return (0);
}

static int	ft_cd_home(t_global *g, char *cmd)
{
	char	*path;

	path = ft_get_env_var_value(g->env, "HOME");
	if (!path)
	{
		handle_cd_err(cmd, g, 3);
		return (1);
	}
	return (free_ptr((void **) &path), chdir(path));
}

static void	handle_cd_err(char *cmd, t_global *g, int code)
{
	char	*value;
	char	*final;

	g->last_exit_code = 1;
	value = ft_strjoin("minishell: ", cmd);
	if (code == 1)
	{
		final = ft_strjoin(value, ": too many arguments\n");
		ft_putstr_fd(final, 2);
	}
	else if (code == 2)
	{
		final = ft_strjoin(value, ": No such file or directory\n");
		ft_putstr_fd(final, 2);
	}
	else if (code == 3)
	{
		final = ft_strjoin(value, ": HOME not set\n");
		ft_putstr_fd(final, 2);
	}
	free_ptr((void **) &value);
	free_ptr((void **) &final);
}
