/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:22:35 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/05 17:36:30 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_cd(t_command *cmd, t_global *g);
static bool	ft_handle_one_arg_cd(char *new_pwd, char *pwd_backup, t_global *g);
static void	handle_cd_err(char *cmd, t_global *g, int code);

int	ft_cd(t_command *cmd, t_global *g)
{
	char	*pwd_backup;
	char	*home;
	bool	result;

	result = false;
	if (cmd->args[2])
	{
		handle_cd_err(cmd->args[0], g, 1);
		return (1);
	}
	pwd_backup = ft_get_env_var_value(g->env, "PWD");
	if (!cmd->args[1])
	{
		home = ft_get_env_var_value(g->env, "HOME");
		if (home)
			result = ft_handle_one_arg_cd(home, pwd_backup, g);
		free_ptr((void **)&home);
	}
	else
		result = ft_handle_one_arg_cd(cmd->args[1], pwd_backup, g);
	free_ptr((void **)&pwd_backup);
	if (result == true)
		return (1);
	return (0);
}

/**
 * Function should switch pwd to new_pwd value, updating env
 * variables: $PWD and $OLDPWD
 *
 */
static bool	ft_handle_one_arg_cd(char *new_pwd, char *pwd_backup, t_global *g)
{
	t_list	*ptr;
	char	*new_directory;

	ptr = NULL;
	if (chdir(new_pwd) == -1)
	{
		handle_cd_err(new_pwd, g, 2);
		return (1);
	}
	new_directory = ft_getcwd();
	ptr = ft_return_env_list_node_ptr(g->env, "OLDPWD");
	if (ptr)
		ft_update_env_var_value(ptr, pwd_backup);
	else
		ft_lstadd_back(&g->env, ft_lstnew(init_env_content("OLDPWD",
					pwd_backup)));
	ptr = ft_return_env_list_node_ptr(g->env, "PWD");
	if (ptr)
		ft_update_env_var_value(ptr, new_directory);
	else
		ft_lstadd_back(&g->env, ft_lstnew(init_env_content("PWD",
					new_directory)));
	return (0);
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
	free_ptr((void **)&value);
	free_ptr((void **)&final);
}
