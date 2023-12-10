/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 19:04:08 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/10 18:03:34 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**format_path(char *env_path)
{
	char	**path;
	char	*tmp;
	int		i;

	i = 0;
	tmp = ft_substr(env_path, 5, ft_strlen(env_path) - 5);
	path = ft_split(tmp, ':');
	ft_free_n_null((void **)&tmp);
	while (path[i])
	{
		tmp = ft_strjoin(path[i], "/");
		ft_free_n_null((void **)&path[i]);
		path[i] = tmp;
		i++;
	}
	return (path);
}

char	*check_for_cmd(char **path, char *cmd, t_fd *fd, t_flags flags)
{
	char	*path_2_cmd;
	char	*tmp;
	int		i;

	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], cmd);
		if (!access(tmp, X_OK))
			path_2_cmd = tmp;
		if (!access(tmp, X_OK))
			break ;
		ft_free_n_null((void **)&tmp);
		i++;
	}
	if (!tmp)
		errors(-1, "Error command not found", fd, flags);
	return (path_2_cmd);
}

char	*find_path(char *cmd, t_fd *fd, t_flags flags)
{
	extern char	**environ;
	char		**path;
	char		*path_2_cmd;
	int			i;

	(void)cmd;
	i = 0;
	path = NULL;
	while (*environ)
	{
		if (!ft_strncmp(*environ, "PATH=", 5))
			break ;
		environ++;
	}
	if (!*environ)
		errors(-1, "Error no PATH found in environ", fd, flags);
	path = format_path(*environ);
	path_2_cmd = check_for_cmd(path, cmd, fd, flags);
	i = 0;
	while (path[i])
		ft_free_n_null((void **)&path[i++]);
	ft_free_n_null((void **)&path);
	return (path_2_cmd);
}

void	exec_cmd(char *cmd, t_fd *fd, t_flags flags)
{
	extern char	**environ;
	char		*path;
	int			pid;
	int			p_fd[2];

	pid = fork();
	errors(pid, "Error creating fork for main cmd", fd, flags);
	errors(pipe(p_fd), "Error creating pipe for main cmd", fd, flags);
	if (pid == 0)
	{
		setup_out_pipe(p_fd, fd, flags);
		path = find_path(cmd, fd, flags);
		if (flags.here_doc)
			errors(execve(path, &fd->in.here_doc, environ), "Exec HD", fd, flags);
		else
			errors(execve(path, NULL, environ), "Execve FD", fd, flags);
//! fd->in.in is not a char **, but a in fd *, necesito recuperar el
//_ contenido de la mamada esa.
	}
	else
	{
		setup_in_pipe(p_fd, fd, flags);
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char *argv[])
{
	char	*path;
	t_flags	flags;
	t_fd	fd;

	if (argc < 2)
		return (1);
	init_flags(&flags);
	fd.in.here_doc = NULL;
	while (argv[1])
	{
		ft_printf("path to cmd: %s\n", path = find_path(argv[1], &fd, flags));
		ft_free_n_null((void **)&path);
		argv++;
	}
	return (0);
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