/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:15:23 by zslowian          #+#    #+#             */
/*   Updated: 2025/01/06 18:50:57 by zslowian         ###   ########.fr       */
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

# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TRIM_SET " \t\n"

// TODO: wouldn't it be better if this is an array?
# define PATH_1 "/bin/"
# define PATH_2 "/usr/bin/"
# define PATH_3 "/usr/.local/bin"

# define PROMPT "\e[0;35mminishell$ \e[0m"


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

typedef struct s_node_for_token
{
	char					*str;
	int						type;
	struct s_node_for_token	*prev;
	struct s_node_for_token	*next;
}	t_token;

typedef struct s_global 
{
	bool		interactive;
	t_token		*token;
	char		*user_input;
	char		**env;
}	t_global;

enum	e_token_types
{
	SPACES = 1,
	WORD,
	VAR,
	PIPE,
	INPUT,
	TRUNC,
	HEREDOC,
	APPEND,
	END
};

enum	e_quotes_status
{
	DEFAULT,
	SQUOTE,
	DQUOTE
};

void	ft_process(t_process **proc);

void	ft_error(t_process ***proc, char **string);
void	ft_clean_up(t_process **proc);

void	ft_get_executable_data(t_executable **executable, char *cmd,
			char *file_name);
void	ft_allocate_execve_argv(t_executable **exe, char *cmd);

//lexer

int	tokenization(t_global *global);
// void	ft_delete_lst_node(t_list *node);
// void	ft_delete_lst(t_list **node, int size);


#endif
