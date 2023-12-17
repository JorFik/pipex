/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <JFikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 20:17:48 by JFikents          #+#    #+#             */
/*   Updated: 2023/12/17 21:27:28 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "pipex_exec.h"

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

char	*check_for_cmd(char **path, char *cmd, t_fd_argvs *fd, t_flags flags)
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

char	*erease_flags(char *cmd)
{
	int		i;
	char	*cmd_no_flags;

	cmd_no_flags = ft_strdup(cmd);
	if (!ft_strchr(cmd_no_flags, ' '))
		return (cmd_no_flags);
	i = ft_strlen(cmd_no_flags);
	while (i && cmd_no_flags[--i] != ' ')
		cmd_no_flags[i] = 0;
	cmd_no_flags[i] = 0;
	if (ft_strchr(cmd_no_flags, ' '))
		return (erease_flags(cmd_no_flags));
	return (cmd_no_flags);
}

char	*find_path(char *cmd, t_fd_argvs *fd, t_flags flags)
{
	extern char	**environ;
	char		**path;
	char		*path_2_cmd;
	int			i;

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
	path_2_cmd = check_for_cmd(path, cmd = erease_flags(cmd), fd, flags);
	ft_free_n_null((void **)&cmd);
	while (path[i])
		ft_free_n_null((void **)&path[i++]);
	ft_free_n_null((void **)&path);
	return (path_2_cmd);
}

void	create_argvs(char *argv[], t_fd_argvs *fd, t_flags flags)
{
	char	**cmd_n_args;
	int		argvs_i;
	int		argv_i;
	int		cmd_i;

	argvs_i = 0;
	argv_i = 2 + flags.here_doc;
	fd->argvs = (char ***)ft_calloc((flags.cmd_count + 1), sizeof(char **));
	if (fd->argvs == NULL)
		errors(-1, "Error creating fd->argvs", fd, flags);
	while (argv_i <= flags.last_cmd)
	{
		cmd_n_args = ft_split(argv[argv_i], ' ');
		cmd_i = 0;
		while (cmd_n_args[cmd_i])
			cmd_i++;
		fd->argvs[argvs_i] = (char **)ft_calloc((cmd_i + 1), sizeof(char *));
		fd->argvs[argvs_i][0] = find_path(argv[argv_i++], fd, flags);
		cmd_i = 0;
		while (cmd_n_args[++cmd_i])
			fd->argvs[argvs_i][cmd_i] = cmd_n_args[cmd_i];
		argvs_i++;
		ft_free_n_null((void **)&cmd_n_args[0]);
		ft_free_n_null((void **)&cmd_n_args);
	}
}
