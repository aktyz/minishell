#include "minishell.h"

void	ft_clean_minishell_env(void *env_content_node);

/**
 * Save free the pointer. The NULL exuation need to be up in the function
 * otherwise it won't work.
 *
*/
void	free_ptr(void *ptr)
{
	if (ptr != NULL)
		free(ptr);
}

/* restore_io:
*	Restores the original standard input and standard output
*	to their original fds of 0 and 1. Used to clear the input/output
*	fds after execution, in preparation for the next set of user commands.
*	Returns 1 if the duplication was successful, 0 if not.
*/
bool	restore_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	if (io->stdin_backup != -1)
	{
		if (dup2(io->stdin_backup, STDIN_FILENO) == -1)
			ret = false;
		close(io->stdin_backup);
		io->stdin_backup = -1;
	}
	if (io->stdout_backup != -1)
	{
		if (dup2(io->stdout_backup, STDOUT_FILENO) == -1)
			ret = false;
		close(io->stdout_backup);
		io->stdout_backup = -1;
	}
	return (ret);
}

/* free_io:
*	Frees the input/output fd structure.
*/
void	free_io(t_io_fds *io)
{
	if (!io)
		return ;
	restore_io(io);
	if (io->heredoc_delimiter)
	{
		unlink(io->infile);
		free_ptr(io->heredoc_delimiter);
	}
	if (io->infile)
		free_ptr(io->infile);
	if (io->outfile)
		free_ptr(io->outfile);
	if (io)
		free_ptr(io);
}

/* free_str_tab:
*	Frees an array of strings.
*/
void	free_str_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
		{
			if (tab[i])
			{
				free_ptr(tab[i]);
				tab[i] = NULL;
			}
			i++;
		}
		free(tab);
		tab = NULL;
	}
}

void	lst_delone_cmd(t_command *lst, void (*del)(void *))
{
	if (lst->command)
		(*del)(lst->command);
	if (lst->args)
		free_str_tab(lst->args);
	if (lst->io_fds)
		free_io(lst->io_fds);
	(*del)(lst);
}

void	lst_clear_cmd(t_command **lst, void (*del)(void *))
{
	t_command	*temp;

	temp = NULL;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		lst_delone_cmd(*lst, del);
		*lst = temp;
	}
}

/* free_data:
*	Frees all of the data used to run a command. If clear_history is true,
*	frees the environment and the command history before returning.
*/
void	free_global(t_global *global, bool clear_history)
{
	if (global && global->user_input)
	{
		free_ptr(global->user_input);
		global->user_input = NULL;
	}
	// if (global && global->token)
	// 	lstclear_token(&global->token, &free_ptr);
	if (global && global->cmd)
		lst_clear_cmd(&global->cmd, &free_ptr);
	// if (clear_history)
	// {
	// 	if (global && global->working_dir)
	// 		free_ptr(global->working_dir);
	// 	if (global && global->old_working_dir)
	// 		free_ptr(global->old_working_dir);
	// 	if (global && global->env)
	// 		free_str_tab(global->env);
	// 	rl_clear_history();
	// }
}



/* exit_shell:
*	Cleanly exits the minishell program by closing all opened
*	fds and freeing all allocated memory.
*/
void	exit_shell(t_global *global, int exno)
{
	if (global)
	{
		// TODO uncomment below once the commands are ready
		// if (global->cmd && global->cmd->io_fds)
			// close_fds(global->cmd, true);
		free_global(global, true);
		if (global && global->env)
			ft_lstclear(&global->env, ft_clean_minishell_env);
	}
	exit(exno);
}

/**
 * Function freeing env_var nodes on the env list
 *
 */
void	ft_clean_minishell_env(void *env_content_node)
{
	t_minishell_env	*content;

	content = (t_minishell_env*) env_content_node;
	if(!content)
		return ;
	if(content->name_value[0])
	{
		free(content->name_value[0]);
		content->name_value[0] = NULL;
	}
	if(content->name_value[1])
	{
		free(content->name_value[1]);
		content->name_value[1] = NULL;
	}
	if (content->name_value)
	{
		free(content->name_value);
		content->name_value = NULL;
	}
	if(content)
		free(content);
	env_content_node = NULL;
}
