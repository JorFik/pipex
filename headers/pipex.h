/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 19:10:36 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/17 23:59:54 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

// ** ---------------------------- LIBRARIES ---------------------------- ** //

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include "libft.h"

//_--------------------------------------------------------------------------_//
// ** ------------------------------ MACROS ------------------------------ ** //

# ifndef PIPE_READ
#  define PIPE_READ 0
# endif

# ifndef PIPE_WRITE
#  define PIPE_WRITE 1
# endif

# ifndef LIMITER
#  define LIMITER 2
# endif

# ifndef TMP_FILE
#  define TMP_FILE "tmpfile.tmp"
# endif

//_--------------------------------------------------------------------------_//
// ** ------------------------ TYPE DEFINITIONS ------------------------- ** //

typedef struct s_fd
{
	int		in;
	int		out;
	char	***argvs;
}	t_fd_argvs;

typedef struct s_flags
{
	int	here_doc;
	int	last_cmd;
	int	cmd_count;
}	t_flags;

//_--------------------------------------------------------------------------_//

// ** ---------------------------- FUNCTIONS ---------------------------- ** //

/**
	@note//_DESCRIPTION
	@brief #### Frees the argvs and closes the file descriptors.
	@brief Checks if the argvs are not NULL, if so frees them and sets them to
		NULL. Closes all the open file descriptors. and returns 0.
	@note//_PARAMETERS
	@param *fd The structure that contains all the file descriptors and argvs.
	@param flags The structure that contains all the flags.
	@note//_RETURN
	@return 0 if successful, exits directly if an error occurs.
 */
int		free_n_close(t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Initializes the flags.
	@brief Sets the flags to 0, sets the last_cmd to the index of the last
		command, and sets the cmd_count to the amount of commands.
	@note//_PARAMETERS
	@param *flags The structure that contains all the flags.
	@param argc The count of arguments from the main function.
 */
void	init_flags(t_flags *flags, int argc);

/**
	@note//_DESCRIPTION
	@brief #### Trims the limiter from the here_doc input.
	@brief Gets the size of the limiter and the size of the line, then sets the
		last character of the line to '\0' and copies the line to a new string
		without the limiter.
	@note//_PARAMETERS
	@param *line The line to be trimmed.
	@param *limiter The limiter to be trimmed.
	@note//_RETURN
	@return The trimmed line.
	@note//_NOTES
	@note The trimmed line is returned as a string that has been allocated, 
		don't forget to free it.
	@note `line` is `free()` and set to `NULL`.
 */
char	*trim_limiter(char *line, char *limiter);

/**
	@note//_DESCRIPTION
	@brief #### Initializes the file descriptors.
	@brief Initialize everything in `*fd` to 0 / NULL, if the first argument is
		`here_doc` it creates a temporary file, calls `get_here_doc` and writes
		its return to the temporary file, then set `fd->in` to the temporary
		file. If the first argument is not `here_doc` it sets `fd->in` to the
		first argument. Then it opens the last argument and sets `fd->out` to
		it with the appropriate permissions depending if it was `here_doc` or
		not.
	@note//_PARAMETERS
	@param *fd The structure that contains all the file descriptors and argvs.
	@param argc The count of arguments from the main function.
	@param **argv The arguments from the main function.
	@param *flag The structure that contains all the flags.
	@note//_NOTES
	@note exits directly if an error occurs.
 */
void	init_fd(t_fd_argvs *fd, int argc, char *argv[], t_flags *flag);

/**
	@note//_DESCRIPTION
	@brief #### Prepares the output file.
	@brief Sets STDOUT to the output file, and closes the pipe.
	@note//_PARAMETERS
	@param pipe The file descriptors of the pipe.
	@param fd This structure contains all the file descriptors, and argvs, is
		used for the fd->out and in case of an error.
	@param flags This structure contains all the flags, is used in case of an
		error.
	@note//_NOTES
	@note exits directly if an error occurs.
 */
void	setup_out_file(int pipe[2], t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Check for errors.
	@brief Checks if `check` is -1, if so prints the error message to stderr,
		frees the `argvs`, closes all the open file descriptors, and exits.
	@note//_PARAMETERS
	@param check The value to be checked.
	@param *msg The error message to be printed.
	@param *fd The structure that contains all the file descriptors and argvs.
	@param flags The structure that contains all the flags.
	@note//_NOTES
	@note exits directly if an error occurs.
 */
void	errors(int check, char *msg, t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### check for the count of arguments.
	@brief Checks if the count of arguments is less than 5, if so prints the
		error message to stderr, and exits.
	@note//_PARAMETERS
	@param argc The count of arguments from the main function.
	@note//_NOTES
	@note exits directly if an error occurs.
 */
void	check_count_of_args(int argc);

/**
	@note//_DESCRIPTION
	@brief #### Executes the commands.
	@brief Creates a pipe, if is the first command it duplicates the input file
		descriptor to STDIN, then forks, if is the child process it sets the
		output to the pipe or the output file depending if is the last command
		, then executes the command. If is the parent process it waits for the
		child process to end, if is not the last command it sets the input to
		the pipe and calls itself with the next command.
	@note//_PARAMETERS
	@param argv_i The index of the command to be executed.
	@param *fd The structure that contains all the file descriptors and argvs.
	@param flags The structure that contains all the flags.
	@note//_NOTES
	@note `argv_i` can be change to a static variable, but it is not necessary.
	@note exits directly if an error occurs.
 */
void	exec_cmd(int argv_i, t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Creates the argvs.
	@brief Creates the argvs, for every command with its corresponding arguments
		the first string per command is the path to the command, then the flags
		and the last string is NULL.
	@note//_PARAMETERS
	@param **argv The arguments from the main function.
	@param *fd The structure that contains all the file descriptors and argvs.
	@param flags The structure that contains all the flags.
	@note//_NOTES
	@note uses `find_path` to find the path to the command.
	@note exits directly if an error occurs.
	@note//_SEE_ALSO
	@see find_path()
	@see ft_split()
	@see errors()
	@see ft_free_n_null()
	@see ft_calloc()
 */
void	create_argvs(char *argv[], t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### `free()` argvs.
	@brief Checks if `fd->argvs` is not NULL, if so `free()`s it and sets it to
		NULL.
	@note//_PARAMETERS
	@param *fd The structure that contains all the file descriptors and argvs.
 */
void	free_argvs(t_fd_argvs *fd);

//_--------------------------------------------------------------------------_//

#endif
/*
_----------------------------- ALLOWED FUNCTIONS -----------------------------_
?	open(char *file path, int flags, (OPT) permissions)
		Opens a file and returns a file descriptor.
?	close(int file descriptor)
		Closes a file descriptor.
?	read(int file descriptor, void *buffer, int number of bytes)
		Reads a file descriptor n bytes, and stores it in the buffer.
?	write(int file descriptor, void *buffer, int number of bytes)
		Writes a file descriptor n bytes from the buffer.
?	malloc(int number of bytes)
		Allocates n bytes of memory and returns a pointer to it.
?	free(void *pointer)
		Frees the memory allocated by malloc.
?	perror(char *msg)
		Prints the error message to stderr.
?	strerror(int error number)
		Returns the error message as a string.
?	access(char *file path, int flags)
		_flags: F_OK (exist), R_OK(readable), W_OK(write), X_OK(execute)
		Checks if the file exists and if the user has the right permissions.
		depending on the flags.
		Returns 0 if the file exists and the user has the right permissions.
		Returns -1 if the file does not exist or the user does not have the
		right permissions.
?	dup(int file descriptor)
		Duplicates a file descriptor.
?	dup2(int old fd, int new fd)
		Duplicates a file descriptor to a specific file descriptor.
!		Does not close the old file descriptor. you must close both at the end.
?	execve(char *path, char **argv, char **envp)
		Executes a program with the given arguments and environment variables.
!		Does not return if successful.
?	exit(int status)
		Exits the program with the given status.
?	fork(void)
		Creates a child process.
_		The child process is an exact copy of the parent process at the moment.
		Returns 0 in the child process.
		Returns the child's pid in the parent process.
?	pipe(int *file descriptors)
		Creates a pipe and stores the file descriptors in the given array.
?	unlink(char *file path)
		Deletes a file.
?	wait(int *status)
		Waits for a child process to end.
		status is set to the child's exit status.
?	waitpid(int pid, int *status, int options)
		Waits for a specific child (given its PID) process to end.
		status is set to the child's exit status.
_		options: 
			WNOHANG (returns immediately if no child has exited)
			WUNTRACED (returns if a child has stopped)
			WCONTINUED (returns if a stopped child has been resumed)
!			You can combine the options with a bitwise OR (|)
*/