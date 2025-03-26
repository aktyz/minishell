#include "minishell.h"


int	g_last_exit_code;


/* env_var_count:
*	Counts how many original environment variables there are.
*	Returns the number of environment variables.
*/
int	env_var_count(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}


/* init_env:
*	Initializes a global variable with the contents of the environment
*	variables inherited from the original shell.
*	Returns 0 on failure, 1 on success.
*/
static bool	init_env(t_global *global, char **env)
{
	int		i;

	global->env = ft_calloc(env_var_count(env) + 1, sizeof * global->env);
	if (!global->env)
		return (false);
	i = 0;
	while (env[i])
	{
		global->env[i] = ft_strdup(env[i]);
		if (!global->env[i])
			return (false);
		i++;
	}
	return (true);
}

/* init_wds:
*	Initializes working directory variables as a safeguard against
*	environment PWD and OLDPWD being unset or otherwise not present
*	in the environment. Used for cd builtin.
*	Returns true if successful, false in case of error.
*/
/*
static bool	init_wds(t_data *data)
{
	char	buff[PATH_MAX];
	char	*wd;

	wd = getcwd(buff, PATH_MAX);
	data->working_dir = ft_strdup(wd);
	if (!data->working_dir)
		return (false);
	if (get_env_var_index(data->env, "OLDPWD") != -1)
	{
		data->old_working_dir = ft_strdup(get_env_var_value(data->env,
					"OLDPWD"));
		if (!data->old_working_dir)
			return (false);
	}
	else
	{
		data->old_working_dir = ft_strdup(wd);
		if (!data->old_working_dir)
			return (false);
	}
	return (true);
}
*/

/* init_global:
*	Initializes the global structure used in parsing and executing user input.
*	Returns true if successful, false in case of error.
*/
bool	init_global(t_global *global, char **env)
{
	if (!init_env(global, env))
	{
		errmsg_cmd("Fatal", NULL, "Could not initialize environment", 1);
		return (false);
	}
    // TODO ucomment when relevant parts are ready
	// if (!init_wds(data))
	// {
	// 	errmsg_cmd("Fatal", NULL, "Could not initialize working directories",
	// 		1);
	// 	return (false);
	// }
	global->token = NULL;
	global->user_input = NULL;

    // TODO uncomment when commands ready
	// global->cmd = NULL;
	// global->pid = -1;
	g_last_exit_code = 0;
	return (true);
}
