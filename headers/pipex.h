/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 19:10:36 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/10 18:15:05 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
//?	open, close, read, write, access, dup, dup2, execve, fork, pipe, unlink
# include <stdlib.h>
//?	malloc, free, exit
# include <stdio.h>
//?	perror
# include <string.h>
//?	strerror
# include <sys/wait.h>
//?	wait, waitpid
# include "libft.h"

# ifndef PIPE_READ
#  define PIPE_READ 0
# endif

# ifndef PIPE_WRITE
#  define PIPE_WRITE 1
# endif

# ifndef LIMITER
#  define LIMITER 2
# endif

typedef union u_input
{
	char	*here_doc;
	int		in;
}	t_input;

typedef struct s_fd
{
	t_input	in;
	int		out;
}	t_fd;

typedef struct s_flags
{
	int	here_doc;
}	t_flags;

//_UTILS
int		free_n_close(t_fd *fd, t_flags flags);
void	init_flags(t_flags *flags);
char	*trim_limiter(char *line, char *limiter);
void	setup_in_pipe(int p_fd[2], t_fd *fd, t_flags flags);
void	setup_out_pipe(int p_fd[2], t_fd *fd, t_flags flags);

//_ERRORS
void	errors(int check, char *msg, t_fd *fd, t_flags flags);
void	check_count_of_args(int argc);

#endif

//! path_2_cmd is alocated in find_path, so it needs to be freed.