/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 19:11:46 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/10 16:45:54 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_flags(t_flags *flags)
{
	flags->here_doc = 0;
}

char	*trim_limiter(char *line, char *limiter)
{
	int		size_limiter;
	int		size_line;
	char	*line_no_limiter;

	size_limiter = ft_strlen(limiter);
	size_line = ft_strlen(line);
	ft_bzero(&line[size_line - 1 - size_limiter], size_limiter + 1);
	line_no_limiter = ft_strdup(line);
	ft_free_n_null((void **)&line);
	return (line_no_limiter);
}

int	free_n_close(t_fd *fd, t_flags flags)
{
	int	check_in;
	int	check_out;

	check_in = 0;
	check_out = 0;
	if (flags.here_doc)
		ft_free_n_null((void **)&fd->in.here_doc);
	else if (fd->in.in > 0)
		check_in = close(fd->in.in);
	if (fd->out > 0)
		check_out = close(fd->out);
	if (check_in == -1 || check_out == -1)
	{
		if (check_in == -1 && check_out == -1)
			perror("Error closing both fds");
		if (check_out == -1)
			perror("Error closing fd out");
		perror("Error closing fd in");
		exit (1);
	}
	return (0);
}

void	setup_out_pipe(int p_fd[2], t_fd *fd, t_flags flags)
{
	close(p_fd[PIPE_READ]);
	errors(dup2(p_fd[PIPE_WRITE], STDOUT_FILENO), "Error dup2 out", fd, flags);
	close(p_fd[PIPE_WRITE]);
}

void	setup_in_pipe(int p_fd[2], t_fd *fd, t_flags flags)
{
	close(p_fd[PIPE_WRITE]);
	errors(dup2(p_fd[PIPE_READ], STDIN_FILENO), "Error dup2 in", fd, flags);
	close(p_fd[PIPE_READ]);
}
