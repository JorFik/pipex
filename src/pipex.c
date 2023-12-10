/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 18:54:08 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/10 18:15:39 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_here_doc(t_fd *fd, int argc, char *argv[], t_flags *flags)
{
	char	*line;
	char	*tmp;
	char	*line_stdin;

	line = NULL;
	tmp = NULL;
	line_stdin = NULL;
	if (argc < 6)
		errors(-1, "Error not enough arguments to use here_doc", fd, *flags);
	flags->here_doc = 1;
	while (ft_strncmp(line_stdin, argv[LIMITER], ft_strlen(argv[LIMITER])))
	{
		ft_free_n_null((void **)&line_stdin);
		ft_printf("pipex here_doc> ");
		line_stdin = get_next_line(0);
		tmp = ft_strjoin(line, line_stdin);
		ft_free_n_null((void **)&line);
		line = tmp;
	}
	ft_free_n_null((void **)&line_stdin);
	return (trim_limiter(line, argv[LIMITER]));
}

void	init_fd(t_fd *fd, int argc, char *argv[], t_flags *flag)
{
	fd->in.here_doc = NULL;
	fd->out = 0;
	if (!ft_strncmp(argv[1], "here_doc", 9))
		fd->in.here_doc = get_here_doc(fd, argc, argv, flag);
	else
		fd->in.in = open(argv[1], O_RDONLY);
	errors(fd->in.in, "Error opening input file", fd, *flag);
	if (flag->here_doc)
		fd->out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd->out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	errors(fd->out, "Error opening output file", fd, *flag);
}

// int	main(int argc, char	*argv[])
// {
// 	t_fd	fd;
// 	t_flags	flags;

// 	check_count_of_args(argc);
// 	init_flags(&flags);
// 	init_fd(&fd, argc, argv, &flags);
// 	return (free_n_close(&fd, flags));
// }

/*
_ALLOWED FUNCTIONS:
?	open, close, read, write,
?	malloc, free, perror,
?	strerror, access, dup, dup2,
?	execve, exit, fork, pipe,
?	unlink, wait, waitpid
*/