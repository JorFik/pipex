/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 19:10:36 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/17 19:57:35 by JFikents         ###   ########.fr       */
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
//_UTILS
//frees alloc memory and closes all fds
int		free_n_close(t_fd_argvs *fd, t_flags flags);

//initializes flags
void	init_flags(t_flags *flags, int argc);

//trims the limiter from here_doc input
char	*trim_limiter(char *line, char *limiter);

//initializes fd and runs if necessary here_doc
void	init_fd(t_fd_argvs *fd, int argc, char *argv[], t_flags *flag);

void	setup_out_file(int pipe[2], t_fd_argvs *fd, t_flags flags);

//_ERRORS
//checks if check is -1, if so prints msg, frees and closes all and exits
void	errors(int check, char *msg, t_fd_argvs *fd, t_flags flags);
//checks if we have at least 4 arg, if not prints error and exits
void	check_count_of_args(int argc);

//_EXEC_CMD
//executes the cmd with infile as input and fd as output
void	exec_cmd(int argv_i, t_fd_argvs *fd, t_flags flags);

void	create_argvs(char *argv[], t_fd_argvs *fd, t_flags flags);

void	init_fd(t_fd_argvs *fd, int argc, char *argv[], t_flags *flag);

void	free_argvs(t_fd_argvs *fd);
//_--------------------------------------------------------------------------_//

#endif

//! path_2_cmd is alocated in find_path, so it needs to be freed.
//! hay que checar argv con el comando porque borro las flags en ereased flags
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