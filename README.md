# Installing & using criterion library

* Run the following command

```
sudo apt-get install libcriterion-dev
```


* Compile and run tests
```
make 
./tester
```

# Running minishell
```
make
./minishell
```



# Main program informations

## Structures used in the program

Structure to keep commands in <what kind of data structure are we using> structure:
```
typedef struct s_command
{
	char				*command;
	char				*path;
	char				**args;
	bool				pipe_output;
	int					*pipe_fd;
	t_io_fds			*io_fds;
	struct s_command	*next;
	struct s_command	*prev;
}	t_command;
```
The idea is that the execution part will iterate through those in order to provide final output.

Structure to keep global state of the program:
```
typedef struct s_data
{
	bool		interactive;
	t_token		*token;
	char		*user_input;
	char		**env;
	char		*working_dir;
	char		*old_working_dir;
	t_command	*cmd;
	pid_t		pid;
}	t_data;
```

## Code organization

I would prefer we create a clear separation as per functionality of our code. Our folders would include:
- builtin
- cleanup
- debug
- examples
- execution
- includes
- libft as git submodule
- parsing
- test_files [with potentially different name to match one word convention of other file folders]
- test

That would leave in the main folder only crucial files:
- .git[xyz] files
- main.c file
- Makefile
- README.md

## For Zyta where to start next time
I need to come up with a list of examples of piped commands that would not run into "bonus" part of the project.
Ie. trying to run `ls *.c` resolves into error because we should not handle wildcards; `cd ./test_files` needs to be implemented first as a built-in to work
Candidates so far:
- ls | wc -w
- cat ./test_files/myfile.txt | grep "error" | wc -w
- ps aux | grep wsl
- du -h | sort -hr
- [x] run single commands in minishell to check if working, then use to develop the piping mechanism between minishell kids
- [x] make sure the parent process closes the pipes in the right place so that it doesn't interfere in the pipes between child processes


## TODO for minishell (aka. our Backlog)
- [x] (Z) create mocs of the `t_data` and `s_command` in order to be able to work on execution part
	- [x] program executes a single command with one argument
	- [x] program executes single command with variable inside `$VAR`
- [ ] (Zytka) rework/tidy up Libft list functions so that we can move ```libft_functions.c``` there
- [ ] replace char ```*args[4]``` in structure with args array and name of the file in the right variable if necessary
- [ ] How to make parent process fail gracefully when child is exiting with ERROR(?)
- [ ] learn AST (Abstract Syntax Three) and if they would be usefull for keeping my processes
- [x] unify ```pipex_child.c``` and ```pipex_parent.c``` into one function
- [x] create a list of different Linux command calls in chain (test cases)
- [x] (Zytka) start working on parsing, the output of the parsing being an array of strings - we have a ft_split() in Libft
- [x] figure out the structure to keep multiple commands -> KISS - list
- [x] (Marlenka) start working on a inbetween pipes string parse into a list node with struct so that our program after compile can execute:<br>
```./minishell "./test_files/infile1 > grep a1 | wc -l >> ./test_files/outfile"```
## Test cases
```./test_files/infile0 "grep a1" "wc -l" ./test_files/outfile```<br>
```./test_files/infile1 "ls -la" "grep minishell" ./test_files/outfile```<br>
```./test_files/infile1 "ls -la" "grep minishell"```<br>
```./test_files/infile1 "not a command" "grep minishell" ./test_files/outfile```<br>

# BASH Notes (Minishell concept notes for allowed functions)

## Usefull resources
<i>[Whitespace functions as a separator between commands and/or variables. Whitespace consists of either spaces, tabs, blank lines, or any combination thereof.](https://tldp.org/LDP/abs/html/special-chars.html#WHITESPACEREF)</i>

## <unistd.h> Library

### close (some examples in man 3)
The  close() function shall deallocate the file descriptor indicated by fildes. To deallocate means to make the file descriptor available for return by subsequent calls to open() or other functions that allocate file descriptors. All outstanding record locks owned by the process on the file associated with the file descriptor shall be removed (that is, unlocked). Syntax:<br>
```
	int close(int fildes);
```
Upon successful completion, 0 shall be returned; otherwise, -1 shall be returned and ```errno``` set to indicate the error.

### read (one example in man 3)
Syntax:<br>
```
	ssize_t read(int fildes, void *buf, size_t nbyte);
```
### write
### access
This function answers a question: can  the  user  who  invoked  me read/write/execute  this  file?

> protip: run with flag F_OK checks if the file exists

### dup
### dup2
### execve
Function to run the bash script. It doesn't work on my computer with WSL

### fork
Clue of the project. Creates a spawned process with it's own copy of memory.
### pipe
Syntax:<br>
```
int pipe(int pipefd[2]);
```
This actually creates two working fds, one for read an second for write.
Those fd's can be assigned to any thread in order to communicate between each other.

### unlink
Simply put deleting the file.

## <stdlib.h> Library

### malloc
### free
### exit

## <stdio.h> Library

### perror
Syntax:<br>
```
void perror(const char *s);
```
The  perror()  function produces a message on standard error describing the last error encountered during a call to a system or library function.

## <string.h> Library

### strerror
Syntax:<br>
```
	char *strerror(int errnum);
```
The  ```strerror()```  function  returns:
	a pointer
		to a string
			that describes the error code
				passed in the argument errnum,
possibly using the LC_MESSAGES part of the current locale to select the appropriate language.
(For example, if errnum is EINVAL, the returned description will be "Invalid argument".)  This  string  must not  be  modified  by  the application, but may be modified by a subsequent call to strerror() or strerror_l().  No other library function, including ```perror(3)```, will modify this string.

## <sys/stat.h> Library

### open (some examples in man 3)
Already used, this option should establish a connection between a file and a file descriptor. Syntax:<br>
```
	int	open(const char *path, int oflag, ...);
```
Exclusive flags (only one can be specified in function):
- ```O_EXEC``` - Open for execute only (non-directory files). The result is unspecified  if  this flag is applied to a directory.
- ```O_RDONLY``` - Open for reading only.
- ```O_RDWR``` - Open for reading and writing. The result is undefined if this flag is applied to a FIFO.
- ```O_SEARCH``` - Open directory for search only. The result is unspecified if this  flag  is  applied to a non-directory file.
- ```O_WRONLY``` - Open for writing only.

Inclusive flags (any combination of the followint may be used):
> seriously, type ```man 3 open``` and scroll down
Upon successful completion, these functions shall open the file and return a non-negative integer representing the file descriptor. Otherwise, these functions shall return -1 and set ```errno``` to indicate the error. If -1 is returned, no files shall be created or modified.

## <sys/wait.h> Library

### wait
### waitpid

# Leak reports/Segmentation fault

```
```
Marlena to do

# renames

data -> global

# todo.txt


* implement and understand missing functions used in parse_user_input
    [x] check_if_var -> check_var
        [x] TESTS
    [x] var_expander
        [x] TESTS
        [x] understand tests
    [x] handle quotes
        [x] TESTS
    [ ] create_commands (skeleton)


* implement functions used by create_commands
    [ ] lst_add_back_cmd
    [ ] parse_word
    [ ] parse_input
    [ ] parse_trunc
    [ ] parse_heredoc
    [ ] parse_append
    [ ] parse_pipe

    [ ] prep_no_arg_commands
