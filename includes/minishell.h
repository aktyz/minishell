/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zslowian <zslowian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:15:23 by zslowian          #+#    #+#             */
/*   Updated: 2025/05/04 08:26:53 by zslowian         ###   ########.fr       */
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
# include <signal.h> // struct sigaction
# include <sys/stat.h> // open()
# include <sys/sysmacros.h>
# include <sys/wait.h> //

# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TRIM_SET " \t\n"

# define ENV_PATH "PATH"
# define ENV_HOME "HOME"

# define ECHO "echo"
# define CD "cd"
# define PWD "pwd"
# define EXPORT "export"
# define UNSET "unset"
# define ENV "env"
# define EXIT "exit"

# define CAT_CAT "cat"
# define CAT_SORT "sort"
# define CAT_UNIQ "uniq"
# define CAT_TEE "tee"
# define CAT_TAIL "tail"
# define CAT_HEAD "head"

# define PROMPT "\e[0;35mminishell$ \e[0m"

# define MINISHELL "minishell: "

# define HEREDOC_NAME "/tmp/.__heredoc__"

typedef struct s_io_fds	t_io_fds;

typedef struct s_node_for_token
{
	char					*str;
	char					*str_backup;
	bool					var_exists;
	int						type;
	int						status;
	bool					join;
	bool					status_request;
	struct s_node_for_token	*prev;
	struct s_node_for_token	*next;
}	t_token;

typedef struct s_command
{
	char				*command;
	char				*path;
	int					args_size;
	char				**args;
	bool				pipe_output;
	int					pipe_fd[2];
	bool				is_builtin;
	bool				status_request;
	pid_t				cmd_pid;
	t_list				*io_fds;
	t_io_fds			*final_io;
}	t_command;

typedef struct s_minishell_env
{
	char	**name_value;
	bool	export;
}	t_minishell_env;

typedef struct s_global
{
	bool		interactive;
	t_token		*token;
	char		*user_input;
	t_list		*env;
	t_list		*cmd;
	bool		is_global;
	int			last_exit_code;
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
	bool	use_heredoc;
	bool	trunc;
	int		fd_in;
	int		fd_out;
};

void		ft_process(t_global *global);
bool		ft_is_our_builtin(t_command *cmd);
void		ft_handle_redirections(t_command *cmd, t_global *g,
				t_command *prev_cmd);
char		**ft_execve_env(t_list *env);

void		ft_split_env_variable(char *name_value, char **var_name,
				char **var_value);
char		*ft_get_env_var_value(char *env_var_name, t_list *env);
char		*resolve_command_path(t_global *g, char *path, char *cmd);
void		ft_execute_child_proc(t_command *cmd, t_global *global,
				pid_t prev_pid);
bool		is_parent_builtin(t_command *command);
void		ft_safe_fork(t_global *g, t_command *cmd);
void		ft_split_child_parent_run(t_global *g, t_command *cmd,
				t_command *prev_cmd);
void		ft_attach_tty(void);
void		ft_chandle_parent_io(t_command *cmd, t_global *g,
				t_command *prev_cmd);
bool		process_env_variable(char *env_var, t_list **list);
void		ft_copy_input_to_final_io(t_io_fds *node, t_command *cmd,
				t_global *g, bool is_heredoc);
void		ft_copy_output_to_final_io(t_io_fds *node, t_command *cmd,
				t_global *g);
void		ft_create_file(char *f_name, t_global *g);
void		ft_calloc_io_node(t_io_fds **ptr, t_global *g);
void		ft_open_final_outfile(t_global *g, t_io_fds **io);
void		ft_open_final_infile(t_global *g, t_io_fds **io);
void		ft_handle_child_io_lst_node(t_global *g, t_command *cmd,
				t_io_fds *node);
void		ft_handle_parent_io_lst_node(t_global *g, t_command *cmd,
				t_io_fds *node);
void		ft_execute_cmd(t_global *g, t_command *cmd, pid_t prev_pid);
void		ft_check_path(char *path, int *error);
void		ft_command_not_found(char *path, int *error);
void		ft_handle_minishell_err(char *cmd, char *error);

//initialization
bool		init_global(t_global *global, char **env);
bool		init_env(t_global *global, char **env);

//cleanup
void		ft_clear_minishell_env(void *env_content_node);
void		ft_clear_token(t_token	**list);

//free

void		free_ptr(void **ptr);
void		free_io(void *io);
void		free_str_tab(char **tab);
void		free_global(t_global *global, bool clear_history);

//lexer
int			tokenization(t_global *global);
bool		input_is_space(char *input);
bool		parse_user_input(t_global *global);
int			ft_strcmp(const char *s1, const char *s2);
// void	ft_delete_lst_node(t_list *node);
// void	ft_delete_lst(t_list **node, int size);

int			check_var(t_token **token_lst);

// errors
void		ft_minishell_perror(char *cmd, int status);
int			errmsg_cmd(char *command, char *detail,
				char *error_message, int error_nb);

// characters
bool		input_is_space(char *input);
int			which_separator(char *str, int i);

// nodes

t_token		*new_node(char *str, int type, int status);
void		add_node(t_token **list, t_token *new_node);
void		delete_node(t_token *node, void (*del)(void **));

// env variables
int			var_expander(t_global *global, t_token **token_lst);
char		*var_expander_heredoc(t_global *global, char *str);

// quotes
int			handle_quotes(t_global *global);

// parse commands
void		create_commands(t_global *global);
void		parse_word(t_command **curr_cmd, t_token **curr_token);
void		parse_input(t_global *global, t_command **last_cmd,
				t_token **token_lst);
void		parse_output(t_global *g, t_command **curr_cmd,
				t_token **curr_token, bool is_trunc);
void		parse_pipe(t_global *g, t_command **curr_cmd, t_token **curr_token);
void		parse_heredoc(t_global *g, t_command **curr_cmd,
				t_token **curr_token);
void		ft_is_status_request(t_token *token, t_command *cmd);
bool		get_heredoc(t_global *global, t_io_fds *io);
char		*get_delim(char *delim, bool *quotes);
char		*get_heredoc_name(void);
int			var_length(char *str);
int			create_args_echo_mode(t_token **token_node, t_command *last_cmd);
int			add_args_echo_mode(t_token **token_node, t_command *last_cmd);
int			create_args_default_mode(t_token **token_node, t_command *last_cmd);
t_token		*lst_new_token(char *str, char *str_backup, int type, int status);
void		lst_add_back_token(t_token **alst, t_token *new_node);
void		lstclear_token(t_token **lst, void (*del)(void **));
bool		contains_space(char *str);
int			count_arguments(t_token *temp);
char		*join_vars(t_token **token_node);
char		**copy_in_new_tab(int len, char **new_tab,
				t_command *last_cmd, t_token *tmp);
void		lstdelone_token(t_token *lst, void (*del)(void **));
bool		quotes_in_string(char *str);
bool		is_next_char_a_sep(char c);
bool		var_between_quotes(char *str, int i);
int			count_len(char *str, int count, int i);
int			replace_var(t_token **token_node, char *var_value, int index);
char		*recover_val(t_token *token, char *str, t_global *global);
char		*replace_str_heredoc(char *str, char *var_value, int index);
void		copy_var_value(char *new_str, char *var_value, int *j);
char		*get_new_token_string(char *oldstr, char *var_value,
				int newstr_size, int index);
void		add_io_infile_data(t_global *global, t_command *cmd, char *f_name);
void		add_io_heredoc_data(t_global *g, t_command *cmd, char *delimiter);
void		ft_clear_minishell_cmd(void *cmd_content_node);
t_command	*lst_last_cmd(t_list *lst);
t_command	*lst_new_cmd(void);

// signals
void		ignore_sigquit(void);
void		set_signals_interactive(void);
void		set_signals_noninteractive(void);

// debug
void		print_token_list(t_token **tokens);
void		print_cmd_list(t_global *global);
void		print_cmd_io(t_command *cmd);

// builtins
int			ft_run_builtin(t_command *cmd, t_global *global);
void		ft_run_parent_builtins(t_command *cmd, t_global *global);
int			ft_echo(char **args);
int			ft_cd(t_command *cmd, t_global *global);
void		ft_exit(t_global *global, char *cmd, int status);
int			ft_pwd(void);
void		ft_export(t_command *cmd, t_global *global);
void		ft_unset(t_command *cmd, t_global *global);
int			ft_env(t_list *env);
void		ft_create_execve_array_entry(char **ptr, t_minishell_env *content);
int			ft_handle_export_arg(char *cmd, t_global *global);
void		ft_handle_export(t_global *global);
void		ft_handle_existing_var(char *cmd, t_minishell_env *content);
bool		is_valid_var_name(char *var_name);
void		ft_update_value_or_add(char *cmd, t_global *global);
void		ft_add_new_env_var(char *var_name, t_global *global);
void		ft_mini_exit_wrapper(t_command *cmd, t_global *g);
void		ft_mini_export_wrapper(t_command *cmd, t_global *global);

#endif
