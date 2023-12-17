/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 19:10:53 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/17 17:51:15 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	errors(int check, char *msg, t_fd_argvs *fd, t_flags flags)
{
	if (check == -1)
	{
		perror(msg);
		free_n_close(fd, flags);
		exit(1);
	}
}

void	check_count_of_args(int argc)
{
	if (argc > 4)
		return ;
	perror("Error not enough arguments");
	exit(1);
}

void	free_argvs(t_fd_argvs *fd)
{
	int	argv_i;
	int	str_i;

	argv_i = 0;
	while (fd->argvs && fd->argvs[argv_i])
	{
		str_i = 0;
		while (fd->argvs[argv_i] && fd->argvs[argv_i][str_i])
			ft_free_n_null((void **)&fd->argvs[argv_i][str_i++]);
		ft_free_n_null((void **)&fd->argvs[argv_i++]);
	}
	ft_free_n_null((void **)&fd->argvs);
}
