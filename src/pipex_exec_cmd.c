/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 19:04:08 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/18 00:30:41 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_exec.h"

void	exec_cmd(int argv_i, t_fd_argvs *fd, t_flags flags)
{
	extern char	**environ;
	int			pid;
	int			p_fd[2];
	int			check;

	errors(pipe(p_fd), "Error creating pipe for cmd", fd, flags);
	if (argv_i == 0)
		errors(dup2(fd->in, STDIN_FILENO), "Error dup2 cmd 1", fd, flags);
	pid = fork();
	errors(pid, "Error creating fork for cmd", fd, flags);
	if (pid == 0)
	{
		if (fd->argvs[argv_i + 1])
			setup_out_pipe(p_fd, fd, flags);
		else
			setup_out_file(p_fd, fd, flags);
		errors(execve(fd->argvs[argv_i][0], fd->argvs[argv_i], environ),
			"Error execve", fd, flags);
	}
	waitpid(pid, &check, 0);
	if (fd->argvs[argv_i + 1])
		setup_in_pipe(p_fd, fd, flags);
	if (fd->argvs[argv_i + 1])
		exec_cmd(argv_i + 1, fd, flags);
}

//!I'm not allowed to use which with execve, this is just for testing.
// {
// 	char		*path;
// 	int			p_fd[2];
// 	int			pid;
// 	char		*argv[3];
// 	extern char	**environ;

// 	path = NULL;
// 	argv[0] = "which";
// 	argv[1] = cmd;
// 	argv[2] = NULL;
// 	errors(pipe(p_fd), "Error creating pipe for finding path", fd, flags);
// 	pid = fork();
// 	errors(pid, "Error creating fork for find_path", fd, flags);
// 	if (pid == 0)
// 		setup_out_pipe(p_fd, fd, flags);
// 	if (pid == 0)
// 		errors(execve("/usr/bin/which", argv, environ), "E-which", fd, flags);
// 	else
// 	{
// 		close(p_fd[PIPE_WRITE]);
// 		waitpid(pid, NULL, 0);
// 		path = get_next_line(p_fd[PIPE_READ]);
// 		close(p_fd[PIPE_READ]);
// 	}
// 	return (path);
// }