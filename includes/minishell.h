/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:15:23 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/11 22:59:51 by zslowian         ###   ########.fr       */
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

# define ENV_PATH "PATH"
# define ENV_HOME "HOME"

# define EXIT "exit"
# define CD "cd"
# define ECHO "echo"

# define PROMPT "\e[0;35mminishell$ \e[0m"

# define HEREDOC_NAME "/tmp/.__heredoc__"

extern int	g_last_exit_code;
typedef struct s_io_fds t_io_fds;

typedef struct s_executable
{
	char	*path; //path
	char	*file_name; //command
	int		execve_argc;
	char	**execve_argv; // args
}	t_executable;

typedef struct s_process
{
	int				pipe_parent[2]; // to_go
	int				pipe_send; // to_go
	int				pipe_receive; // to_go
	int				file_send; // to_go
	int				file_receive; // to_go
	int				in_file_fd; // to_go
	char			*input_data;
	int				out_file_fd;
	char			*output_data;
	int				child_pid;
	t_executable	*executable;
}	t_process;

typedef struct s_node_for_token
{
	char					*str;
	char *str_backup;
	bool var_exists;
	int						type;
	int status;
	bool			join;
	struct s_node_for_token	*prev;
	struct s_node_for_token	*next;
}	t_token;

typedef struct s_command
{
	char				*command;
	char				*path;
	int					args_size;
	char				**args;
	bool				pipe_output; // pipes
	int					pipe_fd[2]; // pipes
	bool				is_builtin;
	pid_t				cmd_pid;
	t_io_fds			*io_fds;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;

typedef struct s_global
{
	bool		interactive;
	t_token		*token;
	char		*user_input;
	char		**env;
	t_command	*cmd;
	bool		is_global;
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

struct s_io_fds
{
	char	*infile;
	char	*outfile;
	char	*heredoc_delimiter;
	bool	heredoc_quotes;
	int	fd_in;
	int	fd_out;
	int	stdin_backup;
	int	stdout_backup;
};

void	ft_process(t_global *global);
bool	ft_is_our_builtin(char *cmd);
void	ft_handle_redirections(t_command *cmd);

void	ft_error(t_process ***proc, char **string);
void	ft_clean_up(t_process **proc);

void	ft_get_path_and_args(t_command **exe, char *cmd,
			char *file_name);
void	ft_allocate_execve_argv(t_command **cmd, char *str);

char	*extract_env_var(char *var_name, char **env); //TODO: replace with getenv everywhere
char	*resolve_command_path(char *path, char *cmd);

//initialization


bool	init_global(t_global *global, char **env);
void	init_io(t_command *cmd);

//cleanup

void	free_ptr(void *ptr);
void	free_global(t_global *global, bool clear_history);
void	free_str_tab(char **tab);

void	exit_shell(t_global *global, int exno);

//lexer

int		tokenization(t_global *global);
bool	input_is_space(char * input);
bool	parse_user_input(t_global *global);
// void	ft_delete_lst_node(t_list *node);
// void	ft_delete_lst(t_list **node, int size);

int	check_var(t_token **token_lst);


// errors
int	errmsg_cmd(char *command, char *detail, char *error_message, int error_nb);


// env variables

int		var_expander(t_global *global, t_token **token_lst);
char	*var_expander_heredoc(t_global *global, char *str);

// quotes

int handle_quotes(t_global *global);

// parse commands
void	create_commands(t_global *global, t_token *token);
void	parse_word(t_command **cmd, t_token **token_lst);
void	parse_input(t_command **last_cmd, t_token **token_lst);
void	parse_trunc(t_command **last_cmd, t_token **token_lst);
void	parse_append(t_command **last_cmd, t_token **token_lst);
void	parse_pipe(t_command **last_cmd, t_token **token_lst);
void	parse_heredoc(t_global *global, t_command **last_cmd, t_token **token_lst);
t_command	*lst_last_cmd(t_command *cmd);
t_command	*lst_new_cmd(bool value);

// signals

void		ignore_sigquit(void);
void		set_signals_interactive(void);
void		set_signals_noninteractive(void);

// debug

void	print_token_list(t_token **tokens);
void	print_cmd_list(t_global *global);

// builtins
void	ft_run_builtin(t_command *cmd, t_global *data);
void	ft_echo(char **args);
void	ft_cd(t_command *cmd);
void	ft_exit(t_global *data);
void	ft_pwd(void);

// Test functions
void	run_tests(char **env);
void	test_ft_echo();
void	test_extract_env_var(char **env);
void	test_resolve_command_path(char **env);
void	test_single_cmd(t_global *global);


#endif
