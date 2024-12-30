
# TODO for minishell
- [x] unify ```pipex_child.c``` and ```pipex_parent.c``` into one function
- [ ] create a list of different Linux command calls in chain (test cases)
- [ ] start working on parsing, the output of the parsing being a simplified procecss structure:
```
typedef struct s_process
{
	char	*cmd_with_flags; //I already have functions to split into array
	int		in_fd;
	int		out_fd;
	int		child_pid;
}	t_process;
```
- [ ] move ```ft_get_executable_data()``` into the t_process structure creation, with table as an output keept in structure
- [ ] replace char ```*args[4]``` in structure with args array and name of the file in the right variable if necessary
- [ ] learn BST and if they would be usefull for keeping my processes
- [ ] figure out the structure to keep multiple commands
- [ ] How to make parent process fail gracefully when child is exiting with ERROR(?)

# Terminal tests commands
```./pipex infile1 "blahblah a1" "grep pipex" outfile```<br>
```./pipex infile1 "ls -la" "grep pipex"```<br>
```./pipex infile0 "grep a1" "wc -l" outfile```<br>
```./pipex infile1 "ls -la" "grep pipex" outfile```<br>

# TODO
- [x] figure out how to free memory from a string to resolve my cleak even after my program clean-up function (leak des at the bottom of this file)
- [x] norminette on main program structure
- [x] how to handle the save of the data passed from child to parent - it will have unknown size... and structure depending on the commands called
- [x] check if ```input``` variable prints correctly in ```ft_child_process()``` line 43
- [x] figure out how to trigger excve process in child and then push the output into the pipe
- [x] find the bug overriding the name of the input in void	ft_child_process(t_pipex **pipex)
- [x] adapt your functions to the parent process
- [x] how to make execve() function aware of the input data?
	ie. handle properly: ```./pipex infile "grep a1" ls outfile``` output should be the <i>same</i> as ```< infile grep a1```
- [x] Difficult norm
- [x] Valgrind on proper exit
- [x] Valgrind on Error
- [x] //TODO: check if it still works without the else clause in ```pipex.c``` -> yes it works
- [x] //TODO: check if it still works with this out? in ```pipex_parent.c``` -> yest it works

# Libft TODO
- [x] OOO: create a more comprehensive trim function: ```ft_trim("ls   -l")``` resulted in ```"ls   -l"``` instead of ```"ls -l"```
- [x] fix properly and test ft_strjoin in libft so that you can use it
- [x] gnl to test and fix - instead of "My first line read?" it reads "rst line read?r" skipping the first BUFFER size of the input

# BASH Notes
<i>[Whitespace functions as a separator between commands and/or variables. Whitespace consists of either spaces, tabs, blank lines, or any combination thereof.](https://tldp.org/LDP/abs/html/special-chars.html#WHITESPACEREF)</i>

# Bonus/Minishell concept notes
## Pipes between processes
In my basic pipex, parent process opened a pipe from which it later read. I will follow this read-write direction in bonus/minishell:
- pipe_parent[2] will contain fds from which:
	- parent process will read
	- child process will write
- pipe_child[2] will contain fds from which:
	- child process will read
	- parent process will write
Pipe_child still needs some rethinking once we figure out the structure to hold our processes in the right execution order.

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