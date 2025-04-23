
#include "minishell.h"

static void	ft_delete_env_node(t_list *node, t_global *global);

void	ft_unset(t_command *cmd, t_global *global)
{
	t_minishell_env	*content;
	t_list			*ptr;
	int				len;

	ptr = global->env;
	content = (t_minishell_env *) ptr->content;
	len = ft_strlen(content->name_value[0]);
	while (content && ft_strcmp(cmd->args[1], content->name_value[0]))
	{
		ptr = ptr->next;
		content = (t_minishell_env *) ptr->content;
	}
	if (content)
		ft_delete_env_node(ptr, global);
}

static void	ft_delete_env_node(t_list *node, t_global *global)
{
	t_list	*prev;
	t_list	*current;
	bool	is_removed;

	is_removed = false;
	if (!node || !global || !global->env)
		return;
	if (global->env == node)
	{
		global->env = node->next;
		free(((t_minishell_env *)node->content)->name_value[0]);
		free(((t_minishell_env *)node->content)->name_value[1]);
		free(node->content);
		free(node);
		is_removed = true;
		return;
	}
	if (!is_removed)
	{
		prev = NULL;
		current = global->env;
		while (current && current != node)
		{
			prev = current;
			current = current->next;
		}
		if (current == node)
		{
			prev->next = current->next;
			free(((t_minishell_env *)current->content)->name_value[0]);
			free(((t_minishell_env *)current->content)->name_value[1]);
			free(current->content);
			free(current);
		}
		is_removed = true;
	}
	if (is_removed)
	{
		current = global->env;
		while (current)
		{
			current->lst_size--;
			current = current->next;
		}
	}
}
