/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 19:10:53 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/05 21:00:43 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	errors(int check, char *msg, t_fd *fd, t_flags flags)
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
