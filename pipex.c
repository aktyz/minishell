/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:13:54 by zslowian          #+#    #+#             */
/*   Updated: 2024/12/30 17:09:18 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int			main(int argc, char *argv[]);
void		ft_clean_up(t_process **pipex);
void		ft_error(t_process ***pipex, char **string);
static void	ft_create_process_data(t_process ***pipex);
char		**ft_trim_user_input(char **argv, int argc);

int	main(int argc, char *argv[])
{
	t_process	**process;
	char		**trimmed_argvs;
	// list of pipes strucutres missing
	
	process = ft_calloc(1, sizeof(t_process *));
	*process = ft_calloc(sizeof(t_process), 1);
	if (!(*process))
		ft_error(&process, NULL);
	if (argc != 5)
	{
		ft_printf("Invalid number of arguments given!\n");
		ft_printf("Please try again providing four arguments:\n");
		ft_printf("<infile> <cmd1> <cmd2> <outfile>\n");
		exit(EXIT_FAILURE);
	}
	// while loop missing for separate processes depending on arguments passed from terminal
	/**
	 * Structure I would need for each process:
	 * - string storing command with flags
	 * - something storing the cmd input fd
	 * - something storing the cmd output fd
	 */
	ft_create_process_data(&process);
	trimmed_argvs = ft_trim_user_input(argv, argc);
	// function to start moving through the list of processes missing
	if ((*process)->child_pid == 0)
	{
		(*process)->pipe_send = 1;
		(*process)->pipe_receive = 0;
		ft_get_executable_data(&(*process)->executable, trimmed_argvs[1], trimmed_argvs[0]);
		ft_clear_char_array(&trimmed_argvs, argc - 1);
		ft_process(process);
	}
	else
		waitpid((*process)->child_pid, NULL, 0);
	ft_get_executable_data(&(*process)->executable, trimmed_argvs[2], trimmed_argvs[3]);
	ft_clear_char_array(&trimmed_argvs, argc - 1);
	ft_process(process);
	ft_clean_up(process);
	exit(EXIT_SUCCESS);
}

void	ft_clean_up(t_process **pipex)
{
	int			i;
	t_process	*clean;

	clean = *pipex;
	if (clean->executable->execve_argv)
		ft_clear_char_array(&(clean->executable->execve_argv),
			clean->executable->execve_argc + 1);
	if (clean->executable->infile_name)
		free(clean->executable->infile_name);
	if (clean->executable->outfile_name)
		free(clean->executable->outfile_name);
	if (clean->executable->path)
		free(clean->executable->path);
	if (clean->executable)
		free(clean->executable);
	free(clean);
}

void	ft_error(t_process ***pipex, char **string)
{
	perror(strerror(errno));
	if (**pipex)
		ft_clean_up(*pipex);
	if (string)
		free(string);
	exit(EXIT_FAILURE);
}

static void	ft_create_process_data(t_process ***pipex)
{
	int			c;
	t_process	*process;

	process = **pipex;
	(process)->executable = ft_calloc(1, sizeof(t_executable));
	if (!(process)->executable)
		ft_error(pipex, NULL);
	(process)->pipe_receive = 1;
	c = pipe((process)->pipe_parent);
	if (c == -1)
		ft_error(pipex, NULL);
	(process)->child_pid = (int) fork();
	if ((process)->child_pid == -1)
		ft_error(pipex, NULL);
}

char		**ft_trim_user_input(char **argv, int argc)
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