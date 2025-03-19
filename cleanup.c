#include "minishell.h"



/* free_ptr:
*	Frees a pointer of any type if it is not NULL and sets it to NULL.
*	This avoids accidental double-frees.
*/
void	free_ptr(void *ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
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
	// if (global && global->cmd)
	// 	lst_clear_cmd(&global->cmd, &free_ptr);
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
	}
	exit(exno);
}