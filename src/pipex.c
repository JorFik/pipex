/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 18:54:08 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/17 23:25:45 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_here_doc(t_fd_argvs *fd, int argc, char *argv[], t_flags *flags)
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
	flags->cmd_count --;
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

void	init_fd(t_fd_argvs *fd, int argc, char *argv[], t_flags *flag)
{
	char	*here_doc;

	fd->in = 0;
	fd->out = 0;
	fd->argvs = NULL;
	if (!ft_strncmp(argv[1], "here_doc", 9))
	{
		errors(fd->in = open(TMP_FILE, O_WRONLY | O_CREAT | O_TRUNC,
				0666), "Error creating TMP_FILE, set a different", fd, *flag);
		here_doc = get_here_doc(fd, argc, argv, flag);
		errors(write(fd->in, here_doc, ft_strlen(here_doc)), "Error writing",
			fd, *flag);
		errors(ft_close(&fd->in), "Error closing TMP_FILE", fd, *flag);
		ft_free_n_null((void **) &here_doc);
		errors(fd->in = open(TMP_FILE, O_RDONLY), "Error re-open", fd, *flag);
		fd->out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	}
	else
		fd->in = open(argv[1], O_RDONLY);
	errors(fd->in, "Error opening input file", fd, *flag);
	if (!flag->here_doc)
		fd->out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	errors(fd->out, "Error opening output file", fd, *flag);
}

void	setup_out_file(int pipe[2], t_fd_argvs *fd, t_flags flags)
{
	errors(ft_close(&pipe[PIPE_READ]), "Error closing pipe read", fd, flags);
	errors(ft_close(&pipe[PIPE_WRITE]), "Error closing pipe write", fd, flags);
	errors(dup2(fd->out, STDOUT_FILENO), "Error dup2 out", fd, flags);
}

int	main(int argc, char	*argv[])
{
	t_fd_argvs	fd;
	t_flags		flags;

	check_count_of_args(argc);
	init_flags(&flags, argc);
	init_fd(&fd, argc, argv, &flags);
	create_argvs(argv, &fd, flags);
	exec_cmd(0, &fd, flags);
	return (free_n_close(&fd, flags));
}
