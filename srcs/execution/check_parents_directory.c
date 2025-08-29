/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parents_directory.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:54:49 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/27 16:23:08 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	ft_countdirectory(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (s[i] == '/')
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			count++;
		i++;
	}
	return (count);
}

static int	free_everything_and_return_errnum(t_directories_info *dir,
		t_cmd *cmd, t_data *minishell)
{
	free(dir->path);
	free(dir->parent);
	return (error(errno, dir->file_path, cmd, minishell));
}

static void	init_path(char **path, char *file_path, t_data *minishell)
{
	if (file_path[0] == '/')
		*path = ft_strdup(&file_path[1]);
	else
		*path = ft_strdup(file_path);
	if (path == NULL)
		malloc_error(minishell, NULL, NULL);
}

static int	check_every_parent_directory(t_cmd *cmd, t_data *minishell,
		int is_file_out, t_directories_info *dir)
{
	while (dir->nbr_folder)
	{
		while (dir->path[dir->i] != '\0' && dir->path[dir->i] != '/')
			dir->i++;
		ft_strlcpy(dir->parent, dir->path, dir->i + 1);
		if (dir->nbr_folder == 1 && is_file_out)
		{
			if (access(dir->parent, F_OK | X_OK | W_OK) != 0)
				free_everything_and_return_errnum(dir, cmd, minishell);
		}
		else
		{
			if (access(dir->parent, F_OK | X_OK) != 0)
				free_everything_and_return_errnum(dir, cmd, minishell);
		}
		dir->nbr_folder--;
		dir->i++;
	}
	return (0);
}

int	check_dir(char *file_path, t_cmd *cmd, t_data *minishell, int is_file_out)
{
	t_directories_info	dir;
	int					rtn;

	dir.i = 0;
	dir.parent = NULL;
	dir.file_path = file_path;
	dir.nbr_folder = ft_countdirectory(file_path, '/');
	if (dir.nbr_folder == 0)
		return (0);
	dir.path = NULL;
	init_path(&dir.path, file_path, minishell);
	dir.parent = malloc(sizeof(char) * (ft_strlen(dir.path + 1)));
	if (dir.parent == NULL)
		malloc_error(minishell, dir.path, NULL);
	rtn = check_every_parent_directory(cmd, minishell, is_file_out, &dir);
	if (rtn != 0)
		return (rtn);
	free(dir.path);
	free(dir.parent);
	return (0);
}

// a verif : printf path en sortant de init path
