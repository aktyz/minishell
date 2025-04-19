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
bool	init_env(t_global *global, char **env)
{
	t_list			*list;
	t_minishell_env	*content;
	int				i;

	list = NULL;
	i = 0;
	while (env[i])
	{
		content = ft_calloc(sizeof(t_minishell_env), 1);
		if (!content)
		{
			ft_lstclear(&list, ft_clean_minishell_env);
			return (false);
		}
		content->name_value = ft_split(env[i], '=');
		if (!content->name_value)
		{
			free(content);
			ft_lstclear(&list, ft_clean_minishell_env);
			return (false);
		}
		content->export = true;
		ft_lstadd_back(&list, ft_lstnew(content));
		i++;
	}
	global->env = list;
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
	global->is_global = true;
	global->cmd = NULL;
	// global->pid = -1;
	g_last_exit_code = 0;
	return (true);
}




/* init_io:
*	Initializes a structure with default values to contain
*	infile and outfile information for a command.
*/
void	init_io(t_command *cmd)
{
	if (!cmd->io_fds)
	{
		cmd->io_fds = malloc(sizeof * cmd->io_fds);
		if (!cmd->io_fds)
			return ;
		cmd->io_fds->infile = NULL;
		cmd->io_fds->outfile = NULL;
		cmd->io_fds->heredoc_delimiter = NULL;
		cmd->io_fds->heredoc_quotes = false;
		cmd->io_fds->fd_in = -1;
		cmd->io_fds->fd_out = -1;
		cmd->io_fds->stdin_backup = -1;
		cmd->io_fds->stdout_backup = -1;
	}
}

void	ft_clean_up(t_process **proc)
{
	int			i;
	t_process	*clean;

	clean = *proc;
	if (clean->executable->execve_argv)
		ft_clear_char_array(&(clean->executable->execve_argv),
			clean->executable->execve_argc + 1);
	if (clean->executable->file_name)
		free(clean->executable->file_name);
	if (clean->executable->path)
		free(clean->executable->path);
	if (clean->executable)
		free(clean->executable);
	free(clean);
}

void	ft_error(t_process ***proc, char **string)
{
	perror(strerror(errno));
	if (**proc)
		ft_clean_up(*proc);
	if (string)
		free(string);
	exit(EXIT_FAILURE);
}

static void	ft_create_process_data(t_process ***proc)
{
	int			c;
	t_process	*process;

	process = **proc;
	(process)->executable = ft_calloc(1, sizeof(t_executable));
	if (!(process)->executable)
		ft_error(proc, NULL);
	(process)->pipe_receive = 1;
	c = pipe((process)->pipe_parent);
	if (c == -1)
		ft_error(proc, NULL);
	(process)->child_pid = (int) fork();
	if ((process)->child_pid == -1)
		ft_error(proc, NULL);
}

char	**ft_trim_user_input(char **argv, int argc)
{
	int		i;
	int		j;
	char	**result;

	i = 1;
	j = 0;
	result = ft_calloc(sizeof(char *), argc - 1);
	while (i < argc)
	{
		result[j] = ft_strtrim(argv[i], TRIM_SET);
		i++;
		j++;
	}
	return (result);
}
