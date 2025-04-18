
# include "minishell.h"

char		**ft_execve_env(t_list *env);
static void	ft_sort_export_list(t_list **list);

void	ft_export(t_command *cmd, t_global *global)
{
	t_list			*env;
	t_minishell_env	*content;

	if (cmd->args[1])
	{
		//ft_export_env_var(global);
		return ;
	}
	else
	{
		env = global->env;
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
		j++;
		env = env->next;
	}
	return (execve_env);
}

static void	ft_sort_export_list(t_list **list)
{}
