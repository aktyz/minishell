/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/06 19:23:09 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void		ft_clean_up(t_process **proc);
void		ft_error(t_process ***proc, char **string);
static void	ft_create_process_data(t_process ***proc);
char		**ft_trim_user_input(char **argv, int argc);

void		print_tokens(t_token *list);
void		minishell_interactive(t_global *global);




// NOTE: comment this function to run test cases from tests.c
// criterion test runner adds its own main function

/***/
int	main(int ac, char **av, char **env)
{

	t_global	global; // NOTE originally data
	if (!init_global(&global, env))
		exit_shell(NULL, EXIT_FAILURE);
	minishell_interactive(&global);
	return (0);
}
/**/

void	print_tokens(t_token *list)
{
	t_token	*temp;

	temp = list;
	while (temp)
	{
		printf("%s \n", temp->str);
		temp = temp->next;
	}
}

void	minishell_interactive(t_global *global)
{
	while (1)
	{
		// set_signals_interactive();
		global->user_input = readline(PROMPT);
		// set_signals_noninteractive()
		if (parse_user_input(global)) {
			printf("parsed user input\n");
			print_token_list(&global->token);
		}
			//g_last_exit_code = execute(global);
		//else
		//	g_last_exit_code = 1;

		free_global(global, false);
		global->token = NULL;
		/*
		print_tokens(global->token);
		*/
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

