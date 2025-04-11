#include "minishell.h"

// NOTE TODO this should be inside libft
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static void	initialize_cmd(t_command **cmd)
{
    // TODO use memset with 0 here?
	(*cmd)->command = NULL;
	(*cmd)->path = NULL;
	(*cmd)->args = NULL;
	(*cmd)->pipe_output = false;
	(*cmd)->pipe_fd[0] = -1;
	(*cmd)->pipe_fd[1] = -1;
	(*cmd)->cmd_pid = -1;
	(*cmd)->is_builtin = false;
	(*cmd)->prev = NULL;
	(*cmd)->next = NULL;
}

t_command	*lst_new_cmd(bool value)
{
	t_command	*new_node;

	new_node = (t_command *)malloc(sizeof(t_command));
	if (!(new_node))
		return (NULL);
	ft_memset(new_node, 0, sizeof(t_command));
	new_node->pipe_output = value; //@marlenasn this line is overwritten by the line 22 in every case
	// please check if we need to save "value" to leverage it later
	initialize_cmd(&new_node);
	return (new_node);
}

void	lst_add_back_cmd(t_command **alst, t_command *new_node)
{
	t_command	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

/**
 * Returns the last command of the user input
 *
 */
t_command	*lst_last_cmd(t_command *cmd)
{
	while (cmd->next != NULL)
		cmd = cmd->next;
	return (cmd);
}


static void	prep_no_arg_commands(t_global *global)
{
	t_command	*cmd;

	if (!global || !global->cmd)
		return ;
	cmd = global->cmd;
	while (cmd && cmd->command)
	{
		if (!cmd->args)
		{
			cmd->args = malloc(sizeof * cmd->args * 2);
			cmd->args[0] = ft_strdup(cmd->command);
			cmd->args[1] = NULL;
		}
		cmd = cmd->next;
	}
	cmd = lst_last_cmd(global->cmd);
}



t_token	*lst_new_token(char *str, char *str_backup, int type, int status)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token) * 1);
	if (!(new_node))
		return (NULL);
	new_node->str = str;
	new_node->str_backup = str_backup;
	new_node->var_exists = false;
	new_node->type = type;
	new_node->status = status;
	new_node->join = false;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	lst_add_back_token(t_token **alst, t_token *new_node)
{
	t_token	*start;

	start = *alst;
	if (start == NULL)
	{
		*alst = new_node;
		return ;
	}
	if (alst && *alst && new_node)
	{
		while (start->next != NULL)
			start = start->next;
		start->next = new_node;
		new_node->prev = start;
	}
}

void	lstdelone_token(t_token *lst, void (*del)(void *))
{
	if (del && lst && lst->str)
	{
		(*del)(lst->str);
		lst->str = NULL;
	}
	if (del && lst && lst->str_backup)
	{
		(*del)(lst->str_backup);
		lst->str_backup = NULL;
	}
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	free_ptr(lst);
}

void	lstclear_token(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;

	tmp = NULL;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		lstdelone_token(*lst, del);
		*lst = tmp;
	}
}

static bool	contains_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			return (true);
		i++;
	}
	return (false);
}


int	count_arguments(t_token *temp)
{
	int	i;

	i = 0;
	while (temp && (temp->type == WORD || temp->type == VAR))
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

/*
**  This function fills the array of arguments of the last_cmd by default mode:
**    - It allocates the array of arguments thanks to the count_args function
**    - It loops through the tokens list while the nodes are of type
**        VAR or WORD, and fills last_cmd->args[i] with the current token
*/

int	create_args_default_mode(t_token **token_node, t_command *last_cmd)
{
	int		i;
	int		nb_args;
	t_token	*temp;

	i = 0;
	temp = *token_node;
	nb_args = count_arguments(temp);
	last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
	if (!last_cmd->args)
		return (1);
	temp = *token_node;
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (temp->type == WORD || temp->type == VAR)
	{
		last_cmd->args[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	last_cmd->args[i] = NULL;
	*token_node = temp;
	return (0);
}

static char	**copy_default_in_new_tab(
	int len, char **new_tab, t_command *last_cmd, t_token **tk_node)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = *tk_node;
	while (i < len)
	{
		new_tab[i] = last_cmd->args[i];
		i++;
	}
	while (temp->type == WORD || temp->type == VAR)
	{
		new_tab[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	new_tab[i] = NULL;
	return (new_tab);
}


/*
**  This function joins all the tokens of a quoted sentence
**  (ex " Hello $user") that was previously split.
**  To join them, the tokens have to be of type VAR and the join
**  setting must be set to true (the quotes are implied)
**  The function is only used when the command is "echo".
**
**  ex: The strings -> "Hello" "  " "world"
**      become "Hello  world"
*/

char	*join_vars(t_token **token_node)
{
	t_token	*temp;
	char	*str;

	temp = *token_node;
	str = ft_strdup(temp->str);
	while (temp->type == VAR && temp->next->type == VAR
		&& temp->next->join == true)
	{
		str = ft_strjoin(str, temp->next->str);
		temp = temp->next;
	}
	*token_node = temp;
	return (str);
}

/*
**  This function counts the number of arguments in the list of tokens
**  To consider a token or multiple tokens as an argument they must be either
**  a WORD or a VAR and if they	temp = *token_node;
 are a VAR that has to be joined, we have
**  to loop through all the tokens that check these conditions
**  (type == VAR and join == true) before counting them as one argument
*/

int	count_args(t_token *temp)
{
	int	i;

	i = 0;
	while (temp && (temp->type == WORD || temp->type == VAR))
	{
		if (temp->type == VAR && temp->join == true)
		{
			while (temp->type == VAR && temp->join == true)
				temp = temp->next;
			i++;
		}
		else
		{
			i++;
			temp = temp->next;
		}
	}
	return (i);
}

char	**copy_in_new_tab(int len, char **new_tab,
			t_command *last_cmd, t_token *tmp)
{
	int	i;

	i = 0;
	while (i < len)
	{
		new_tab[i] = last_cmd->args[i];
		i++;
	}
	while (tmp->type == WORD || tmp->type == VAR)
	{
		if (tmp->join == true)
			new_tab[i] = join_vars(&tmp);
		else
			new_tab[i] = ft_strdup(tmp->str);
		i++;
		tmp = tmp->next;
	}
	new_tab[i] = NULL;
	return (new_tab);
}

/* remove_empty_var_args:
**	If a variable does not exist in the environment, the token string
**	will contain "\0". In this case, echo should not print the variable
**	or any spaces before/after it. Therefore the token must be
**	removed before creating/adding echo args.
**	i.e., if variable X does not exist in environment,
**		'echo $X $X $X $USER' should print:
**		'username' (not '  username')
**	However, if the variable exists but its value is empty, the token
**	should not be removed.
*/
void	remove_empty_var_args(t_token **tokens)
{
	t_token	*temp;

	temp = *tokens;
	while (temp->type == WORD || temp->type == VAR)
	{
		if (temp->type == VAR && temp->str[0] == '\0'
			&& temp->var_exists == false)
		{
			temp = temp->next;
			if (temp == (*tokens)->next)
				(*tokens) = (*tokens)->next;
			lstdelone_token(temp->prev, free_ptr);
		}
		else
			temp = temp->next;
	}
}

int	add_args_default_mode(t_token **token_node, t_command *last_cmd)
{
	int		i;
	int		len;
	char	**new_tab;
	t_token	*temp;

	i = 0;
	temp = *token_node;
	while (temp->type == WORD || temp->type == VAR)
	{
		i++;
		temp = temp->next;
	}
	len = 0;
	while (last_cmd->args[len])
		len++;
	new_tab = malloc(sizeof(char *) * (i + len + 1));
	if (!new_tab)
		return (1);
	new_tab = copy_default_in_new_tab(len, new_tab, last_cmd, token_node);
	free(last_cmd->args);
	last_cmd->args = new_tab;
	*token_node = temp;
	return (0);
}

/*
**  This function deals with the specific case when the command is "echo"
**    - It allocates the array of arguments thanks to the count_args function
**    - It loops through the tokens list while the nodes are of type
**        VAR or WORD:
**         * If "bool join = true" in the token structure is true : we join all
**           the tokens of type VAR that have the setting "join = true"
**         * if "join = false" we just fill the last_cmd_>args[i]
**            with the current token.
*/
int	create_args_echo_mode(t_token **token_node, t_command *last_cmd)
{
	int		nb_args;
	t_token	*temp;
	int		i;

	remove_empty_var_args(token_node);
	temp = *token_node;
	nb_args = count_args(temp);
	last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
	if (!last_cmd->args)
		return (1);
	i = 0;
	last_cmd->args[i] = ft_strdup(last_cmd->command);
	i++;
	while (temp->type == WORD || temp->type == VAR)
	{
		if (temp->join == true)
			last_cmd->args[i] = join_vars(&temp);
		else
			last_cmd->args[i] = ft_strdup(temp->str);
		i++;
		temp = temp->next;
	}
	last_cmd->args[i] = NULL;
	*token_node = temp;
	return (0);
}

int	add_args_echo_mode(t_token **token_node, t_command *last_cmd)
{
	int		len;
	int		nb_args;
	char	**new_tab;
	t_token	*temp;

	remove_empty_var_args(token_node);
	temp = *token_node;
	nb_args = count_args(temp);
	len = 0;
	while (last_cmd->args[len])
		len++;
	new_tab = malloc(sizeof(char *) * (nb_args + len + 1));
	if (!new_tab)
		return (1);
	new_tab = copy_in_new_tab(len, new_tab, last_cmd, temp);
	free(last_cmd->args);
	last_cmd->args = new_tab;
	while (temp->type == WORD || temp->type == VAR)
		temp = temp->next;
	*token_node = temp;
	return (0);
}

/*
**  This function fills the arguments in the command structure (command->args)
**  It has two modes:
**    - The "echo mode" if the command is the builtin "echo"
**    - The default mode for all the other cases
*/

int	fill_args(t_token **token_node, t_command *last_cmd)
{
	if (!ft_strcmp(last_cmd->command, "echo"))
	{
		if (!(last_cmd->args))
			return (create_args_echo_mode(token_node, last_cmd));
		else
			return (add_args_echo_mode(token_node, last_cmd));
	}
	else
	{
		if (last_cmd && !(last_cmd->args))
			return (create_args_default_mode(token_node, last_cmd));
		else
			return (add_args_default_mode(token_node, last_cmd));
	}
	return (0);
}

static void	split_var_cmd_token(t_command *last_cmd, char *cmd_str)
{
	t_token		*new_tokens;
	t_token		*tmp;
	char		**strs;
	int			i;

	new_tokens = NULL;
	strs = ft_split(cmd_str, ' ');
	if (!strs)
		return ;
	last_cmd->command = ft_strdup(strs[0]);
	if (strs[1])
		new_tokens = lst_new_token(ft_strdup(strs[1]), NULL, WORD, DEFAULT);
	tmp = new_tokens;
	i = 1;
	while (strs[++i])
		lst_add_back_token(&new_tokens,
			lst_new_token(ft_strdup(strs[i]), NULL, WORD, DEFAULT));
	lst_add_back_token(&new_tokens,
		lst_new_token(NULL, NULL, END, DEFAULT));
	fill_args(&new_tokens, last_cmd);
	lstclear_token(&tmp, &free_ptr);
	free_str_tab(strs);
}

void	parse_word(t_command **cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*last_cmd;

	temp = *token_lst;
	while (temp->type == WORD || temp->type == VAR)
	{
		last_cmd = lst_last_cmd(*cmd);
		if (temp->prev == NULL || (temp->prev && temp->prev->type == PIPE)
			|| last_cmd->command == NULL)
		{
			if (temp->type == VAR && contains_space(temp->str))
				split_var_cmd_token(last_cmd, temp->str);
			else
			{
				last_cmd->command = ft_strdup(temp->str);
				last_cmd->is_builtin = ft_is_our_builtin(last_cmd->command);
			}
			temp = temp->next;
		}
		else
			fill_args(&temp, last_cmd);
	}
	*token_lst = temp;
}


/*	INPUT -> REDIR_IN (<)
	Redirection of input causes the file whose name results from the expansion
	of word to be opened for reading on file descriptor n, or the standard input
	(file descriptor 0) if n is not specified.

	The general format for redirecting input is: [n]<word
*/

bool	remove_old_file_ref(t_io_fds *io, bool infile)
{
	if (infile == true && io->infile)
	{
		if (io->fd_in == -1 || (io->outfile && io->fd_out == -1))
			return (false);
		if (io->heredoc_delimiter != NULL)
		{
			free_ptr(io->heredoc_delimiter);
			io->heredoc_delimiter = NULL;
			unlink(io->infile);
		}
		free_ptr(io->infile);
		close(io->fd_in);
	}
	else if (infile == false && io->outfile)
	{
		if (io->fd_out == -1 || (io->infile && io->fd_in == -1))
			return (false);
		free_ptr(io->outfile);
		close(io->fd_out);
	}
	return (true);
}

/* open_infile:
*	Opens an infile. If an infile was already set, frees it
*	and overwrites it. If a previous infile open failed (file does
*	not exist or permission denied), does not open any further input file.
*	Ex.:
*		< Makefile <README.md cat > test
*			Uses contents of README as input (ignores Makefile)
*		< forbidden <README.md cat > test
*			Permission denied (no README cat)
*/
static void	open_infile(t_io_fds *io, char *file, char *original_filename)
{
	if (!remove_old_file_ref(io, true))
		return ;
	io->infile = ft_strdup(file);
	if (io->infile && io->infile[0] == '\0')
	{
		errmsg_cmd(original_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_in = open(io->infile, O_RDONLY);
	if (io->fd_in == -1)
		errmsg_cmd(io->infile, NULL, strerror(errno), false);
}

void	parse_input(t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_infile(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}


/*
	TRUNC -> Redirection of output.
	The file whose name results from the expansion of word has to be opened
    in writing mode on fd n or the standard output (fd 1) if n is not specified.
    If the file does not exist it is created;
    if it does exist it is truncated to 0 size.

	The general format for redirecting output is: [n]>[|]word
*/

char	*get_relative_path(char *file_to_open)
{
	char	*path;
	char	*ret;

	if (file_to_open[0] == '/')
		return (ft_strdup(file_to_open));
	path = ft_strdup("./");
	ret = ft_strjoin(path, file_to_open);
	printf("PARSING - Get_rel_path function return : %s\n", ret);
	return (ret);
}

/* open_outfile_trunc:
*	Opens an outfile in truncated mode. If an outfile was already set, frees it
*	and overwrites it. If a previous infile or outfile open failed (file does
*	not exist or permission denied), does not open any further output file.
*
*	Ex.:
*		echo hello > forbidden_file > test
*		echo hello >> forbidden_file > test
*		< forbidden_file cat > test
*	In these 3 cases, the test file should not be opened or created.
*/
static void	open_outfile_trunc(t_io_fds *io, char *file, char *var_filename)
{
	if (!remove_old_file_ref(io, false))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0')
	{
		errmsg_cmd(var_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (io->fd_out == -1)
		errmsg_cmd(io->outfile, NULL, strerror(errno), false);
}

void	parse_trunc(t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_outfile_trunc(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}



/* make_str_from_tab:
*	Creates a single string from an array of strings by
*	joining a string to the next.
*	Returns the string.
*/
static char	*make_str_from_tab(char **tab)
{
	char	*str;
	char	*tmp;
	int		i;

	i = -1;
	while (tab[++i])
	{
		tmp = str;
		if (i == 0)
			str = ft_strdup(tab[0]);
		else
		{
			str = ft_strjoin(tmp, tab[i]);
			free_ptr(tmp);
		}
		if (tab[i + 1])
		{
			tmp = str;
			str = ft_strjoin(tmp, " ");
			free_ptr(tmp);
		}
	}
	free_str_tab(tab);
	return (str);
}

/* get_exoanded_var_line:
*	Prepares a line containing '$' for environment variable expansion.
*	Splits the line into words to avoid issues with inexistent
*	environment variables.
*	ex. $USER uses $LANGUAGE -> username uses en_US:en
*	Returns a new line with expanded variables.
*/
static char	*get_expanded_var_line(t_global *global, char *line)
{
	char	**words;
	int		i;

	words = ft_split(line, ' ');
	if (!words)
		return (NULL);
	i = 0;
	while (words[i])
	{
		if (ft_strchr(words[i], '$'))
		{
			words[i] = var_expander_heredoc(global, words[i]);
			if (!words[i])
				return (NULL);
		}
		i++;
	}
	return (make_str_from_tab(words));
}

/* evaluate_heredoc_line:
*	Checks whether the read line should be written to heredoc file.
*	If the line is NULL or the same as the given delimiter, returns false
*	to signify that we should stop reading with readline. Otherwise, returns
*	true.
*/
static bool	evaluate_heredoc_line(t_global *global, char **line,
									t_io_fds *io, bool *ret)
{
	if (*line == NULL)
	{
		errmsg_cmd("warning", "here-document delimited by end-of-file: wanted",
			io->heredoc_delimiter, true);
		*ret = true;
		return (false);
	}
	if (ft_strcmp(*line, io->heredoc_delimiter) == 0)
	{
		*ret = true;
		return (false);
	}
	if (io->heredoc_quotes == false && ft_strchr(*line, '$'))
	{
		*line = get_expanded_var_line(global, *line);
		if (!(*line))
		{
			free_ptr(*line);
			*ret = false;
			return (false);
		}
	}
	return (true);
}


/* fill_heredoc:
*	Copies user input into a temporary file. If user inputs an environment variable
*	like $USER, expands the variable before writing to the heredoc.
*	Returns true on success, false on failure.
*/
bool	fill_heredoc(t_global *global, t_io_fds *io, int fd)
{
	char	*line;
	bool	ret;

	ret = false;
	line = NULL;
	while (1)
	{
		set_signals_interactive();
		line = readline(">");
		set_signals_noninteractive();
		if (!evaluate_heredoc_line(global, &line, io, &ret))
			break ;
		ft_putendl_fd(line, fd);
		free_ptr(line);
	}
	free_ptr(line);
	return (ret);
}

/* get_heredoc:
*	Opens a heredoc awaiting user input.
*	Translates any given variables into	their environment values.
*	Returns false in case of error, true if successful.
*/
bool	get_heredoc(t_global *global, t_io_fds *io)
{
	int		tmp_fd;
	bool	ret;

	ret = true;
	tmp_fd = open(io->infile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	ret = fill_heredoc(global, io, tmp_fd);
	close(tmp_fd);
	return (ret);
}

/* get_heredoc_name:
*	Generates a unique name for the current heredoc.
*	Returns the new heredoc name.
*/
static char	*get_heredoc_name(void)
{
	static int	i;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin(HEREDOC_NAME, number);
	free(number);
	i++;
	return (name);
}

/* get_delim:
*	Returns the heredoc delimiter. Quotes are removed if present
*	around the delimiter, and the quotes boolean is set to true.
*/
static char	*get_delim(char *delim, bool *quotes)
{
	int	len;

	len = ft_strlen(delim) - 1;
	if ((delim[0] == '\"' && delim[len] == '\"')
		|| (delim[0] == '\'' && delim[len] == '\''))
	{
		*quotes = true;
		return (ft_strtrim(delim, "\'\""));
	}
	return (ft_strdup(delim));
}

/* parse_heredoc:
*	Creates a temporary heredoc file which will be filled with
*	user input.
*/
void	parse_heredoc(t_global *global, t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;
	t_io_fds	*io;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	io = cmd->io_fds;
	if (!remove_old_file_ref(io, true))
		return ;
	io->infile = get_heredoc_name();
	io->heredoc_delimiter = get_delim(temp->next->str, &(io->heredoc_quotes));
	if (get_heredoc(global, io))
		io->fd_in = open(io->infile, O_RDONLY);
	// else
	// 	io->fd_in = -1;
	// if (temp->next->next)
	// 	temp = temp->next->next;
	// else
	// 	temp = temp->next;
	// *token_lst = temp;
}


/*
	***APPEND***
	Redirection of output in append mode causes the file whose name results
	from the expansion of word to be opened for appending on file descriptor n,
	or the standard output (fd 1) if n is not specified.
	If the file does not exist it is created.

	The general format for appending output is: [n]>>word.

*/

/* open_outfile_append:
*	Opens an outfile in append mode. If an outfile was already set, frees it
*	and overwrites it. If a previous infile or outfile open failed (file does
*	not exist or permission denied), does not open any further output file.
*
*	Ex.:
*		echo hello > forbidden_file >> test
*		echo hello >> forbidden_file >> test
*		< forbidden_file cat >> test
*	In these 3 cases, the test file should not be opened or created.
*/
static void	open_outfile_append(t_io_fds *io, char *file, char *var_filename)
{
	if (!remove_old_file_ref(io, false))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0' && var_filename)
	{
		errmsg_cmd(var_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (io->fd_out == -1)
		errmsg_cmd(io->outfile, NULL, strerror(errno), false);
}

void	parse_append(t_command **last_cmd, t_token **token_lst)
{
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_outfile_append(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}

void	parse_pipe(t_command **cmd, t_token **token_lst)
{
	t_command	*last_cmd;

	last_cmd = lst_last_cmd(*cmd);
	last_cmd->pipe_output = true;
	lst_add_back_cmd(&last_cmd, lst_new_cmd(false));
	*token_lst = (*token_lst)->next;
}

void	create_commands(t_global *global, t_token *token)
{
	t_token	*temp;

	temp = token;
	if (temp->type == END)
		return ;
	while (temp->next != NULL)
	{
		if (temp == global->token)
			lst_add_back_cmd(&global->cmd, lst_new_cmd(false));
		if (temp->type == WORD || temp->type == VAR)
			parse_word(&global->cmd, &temp);
		else if (temp->type == INPUT)
			parse_input(&global->cmd, &temp);
		else if (temp->type == TRUNC)
			parse_trunc(&global->cmd, &temp);
		else if (temp->type == HEREDOC)
			parse_heredoc(global, &global->cmd, &temp);
		else if (temp->type == APPEND)
			parse_append(&global->cmd, &temp);
		else if (temp->type == PIPE)
			parse_pipe(&global->cmd, &temp);
		else if (temp->type == END)
			break ;
	}
	prep_no_arg_commands(global);
}
