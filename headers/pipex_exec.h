/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 20:19:31 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/17 23:03:45 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_EXEC_H
# define PIPEX_EXEC_H

// ** ---------------------------- LIBRARIES ---------------------------- ** //

# include "pipex.h"

//_--------------------------------------------------------------------------_//

//* ------------------------------ FUNCTIONS ------------------------------ *//
/**
	@note//_DESCRIPTION
	@brief #### Prepares the pipe for input.
	@brief Closes the write end of the pipe, duplicates the read end of the pipe
		to the standard input, and closes the read end of the pipe.
	@note//_PARAMETERS
	@param pfd[2] The file descriptors of the pipe.
	@param fd This structure contains all the file descriptors, is used in case
		of an error to close all the open file descriptors.
	@param flags This structure contains all the flags, is used in case of an
		error to check which variables need to be freed.
 */
void	setup_in_pipe(int p_fd[2], t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Prepares the pipe for output.
	@brief Closes the read end of the pipe, duplicates the write end of the pipe
		to the standard output, and closes the write end of the pipe.
	@note//_PARAMETERS
	@param pfd[2] The file descriptors of the pipe.
	@param fd This structure contains all the file descriptors, is used in case
		of an error to close all the open file descriptors.
	@param flags This structure contains all the flags, is used in case of an
		error to check which variables need to be freed.
 */
void	setup_out_pipe(int p_fd[2], t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Finds the path of the `cmd` and returns it.
	@brief Gets the `PATH` from the environment variables, formats it to an 
		array of strings, formats the `cmd` to be without flags, so it can be
		check if the `cmd` is any of the paths and returns the the full path to
		the `cmd`.
	@note//_PARAMETERS
	@param path The array of paths.
	@param cmd The command to be searched for.
	@param fd This structure contains all the `file descriptors`, is used in
		case of an error to `close()` all the open `file descriptors`.
	@param flags This structure contains all the flags, is used in case of an
		error to check which variables need to be `free()`.
	@note//_RETURN_VALUE
	@return The full path to `cmd`.
	@note//_NOTES
	@note The path to the `cmd` is returned as a `string` that has been 
		allocated, don't forget to `free()` it.
 */
char	*find_path(char *cmd, t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Checks for the `cmd` in the given `path` and returns it
	@brief Checks if the `cmd` is any of the paths in `**path` and returns the 
		path to the `cmd`.
	@note//_PARAMETERS
	@param cmd The command to be searched for.
	@param fd This structure contains all the `file descriptors`, is used in
		case of an error to `close()` all the open `file descriptors`.
	@param flags This structure contains all the flags, is used in case of an
		error to check which variables need to be `free()`.
	@note//_RETURN_VALUE
	@return The path to the cmd
	@note//_NOTES
	@note The path to the `cmd` is returned as a `string` that has been 
		allocated, don't forget to `free()` it.
 */
char	*check_for_cmd(char **path, char *cmd, t_fd_argvs *fd, t_flags flags);

/**
	@note//_DESCRIPTION
	@brief #### Formats `env_path` to an array of strings.
	@brief Formats `env_path` to an array of strings, each string is a different
		path.
	@note//_PARAMETERS
	@param env_path The `PATH` from the environment variables.
	@note//_RETURN_VALUE
	@return The `PATH` as an array of strings.
	@note//_NOTES
	@note The `PATH` is returned as an array of strings that has been 
		allocated, don't forget to `free()` it.
	@note It uses as delimiter `':'`, and it ignores the first 5 characters of
		`env_path` because they are `PATH=`.
 */
char	**format_path(char *env_path);

//_--------------------------------------------------------------------------_//
#endif