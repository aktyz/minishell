/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:15:23 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/03 19:26:04 by zslowian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>  // errno
# include <fcntl.h> // O_RDONLY, O_CREAT
# include <stdio.h>  // perror()
# include <stdlib.h> // exit()
# include <string.h> // strerror()
# include <unistd.h> // pipe() fork()
# include <sys/stat.h> // open()
# include <sys/wait.h> //

# define TRIM_SET " \t\n"

// TODO: wouldn't it be better if this is an array?
# define PATH_1 "/bin/"
# define PATH_2 "/usr/bin/"
# define PATH_3 "/usr/.local/bin"

typedef struct s_executable
{
	char	*path;
	char	*file_name;
	int		execve_argc;
	char	**execve_argv;
}	t_executable;

typedef struct s_process
{
	int				pipe_parent[2];
	int				pipe_send;
	int				pipe_receive;
	int				file_send;
	int				file_receive;
	int				in_file_fd;
	char			*input_data;
	int				out_file_fd;
	char			*output_data;
	int				child_pid;
	t_executable	*executable;
}	t_process;

void	ft_process(t_process **proc);

void	ft_error(t_process ***proc, char **string);
void	ft_clean_up(t_process **proc);

void	ft_get_executable_data(t_executable **executable, char *cmd,
	char *file_name);
void	ft_allocate_execve_argv(t_executable **exe, char *cmd);

char	**ft_lst_to_arr(t_list *argv);
void	ft_delete_lst_node(t_list *node);
void	ft_delete_lst(t_list **node, int size);

#endif