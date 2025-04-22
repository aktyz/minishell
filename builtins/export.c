
# include "minishell.h"

char		**ft_execve_env(t_list *env);
static void	ft_sort_export_list(t_list **list);
static void	ft_handle_existing_var(t_command *cmd, t_minishell_env	*content);

void	ft_export(t_command *cmd, t_global *global)
{
	t_list			*env;
	t_minishell_env	*content;
	char			*var_name;

	env = global->env;
	if (cmd->args[1])
	{
		var_name = cmd->args[1];
		while (env && env->content)
		{
			content = (t_minishell_env*) env->content;
			if (ft_strncmp(var_name, content->name_value[0], ft_strlen(content->name_value[0])) == 0) //var already exists
				return (ft_handle_existing_var(cmd, content));
			env = env->next;
		}
		if (env->content == NULL) // var doesn't exxist -> add it
		{
			content = ft_calloc(sizeof(t_minishell_env), 1);
			if (!content)
				return ;
			content->name_value = ft_calloc(sizeof(char*), 2);
			content->name_value = ft_split(var_name, '='); //TODO: replace with function spliting on the first occurance of '='
			content->export = true;
			ft_lstadd_back(&global->env, ft_lstnew(content));
		}
		return ;
	}
	else // export with no args or parameters
	{
		ft_sort_export_list(&env); //TODO
		while (env && env->content)
		{
			content = (t_minishell_env*) env->content;
			if (content->export && content->name_value)
			{
				if(!content->name_value[1])
					ft_printf("declare -x %s=\"\"\n", content->name_value[0]);
				else
					ft_printf("declare -x %s=\"%s\"\n", content->name_value[0],
						content->name_value[1]);
			}
			env = env->next;
		}
	}
}

char	**ft_execve_env(t_list *env)
{
	char			**execve_env;
	t_minishell_env	*content;
	char			*env_var;
	int				i;
	int				j;

	execve_env = ft_calloc(sizeof(char *), env->lst_size + 1);
	if (!execve_env)
		return (NULL);
	j = 0;
	while(env && env->content)
	{
		content = (t_minishell_env*) env->content;
		if (content->export && content->name_value)
		{
			env_var = ft_calloc(sizeof(char), ft_strlen(content->name_value[0])
				+ ft_strlen(content->name_value[1]) + 2);
			i = ft_strlcpy(env_var, content->name_value[0], ft_strlen(content->name_value[0]));
			env_var[i] = '=';
			ft_strlcpy(env_var + i + 1, content->name_value[1], ft_strlen(content->name_value[1]));
			execve_env[j] = env_var;
		}
		if (execve_env[j])
			j++;
		env = env->next;
	}
	return (execve_env);
}

static void	ft_sort_export_list(t_list **list)
{}

static void	ft_handle_existing_var(t_command *cmd, t_minishell_env *content)
{
	free_ptr(content->name_value[0]);
	free_ptr(content->name_value[1]);
	content->name_value = ft_split(cmd->args[1], '='); //TODO: replace with function spliting on the first occurance of '='
	content->export = true;
}
