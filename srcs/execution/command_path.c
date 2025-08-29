/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 16:51:57 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/19 09:52:44 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	check_path(t_cmd *cmd, t_data *minishell, char *path)
{
	int	error_code;

	if (access(path, F_OK) == -1)
		return (0);
	if (access(path, F_OK | X_OK) == -1)
	{
		error_code = errno;
		free(path);
		exit(error(error_code, cmd->cmd_name, cmd, minishell));
	}
	else
		return (1);
}

static void	init_correct_path(t_cmd *cmd, t_data *minishell, char *path)
{
	cmd->cmd_path = ft_substr(path, 0, ft_strlen(path));
	if (cmd->cmd_path == NULL)
		exit(error(MALLOC_ERROR, cmd->cmd_name, cmd, minishell));
	free(path);
}

void	find_cmd_path_with_path(t_cmd *cmd, t_data *minishell)
{
	char	**possible_path_tab;
	int		i;
	char	*path;

	i = 0;
	possible_path_tab = ft_split(get_path_from_env(minishell), ':');
	if (possible_path_tab == NULL)
		exit(error_fct(MALLOC_ERROR, cmd->cmd_name, cmd, minishell));
	while (possible_path_tab[i])
	{
		path = create_path(possible_path_tab[i], "/", cmd->cmd_name);
		if (path == NULL)
			malloc_error(minishell, NULL, possible_path_tab);
		if (check_path(cmd, minishell, path))
		{
			init_correct_path(cmd, minishell, path);
			break ;
		}
		free(path);
		i++;
	}
	ft_free_split(possible_path_tab);
}

char	*create_path(char *s1, char *s2, char *s3)
{
	char	*newstring;
	int		i;
	int		j;
	int		length;

	if (!s1 || !s2 || !s3)
		return (NULL);
	length = ft_strlen((char *)s1) + ft_strlen((char *)s2)
		+ ft_strlen((char *)s3);
	newstring = malloc(sizeof(char) * (length + 1));
	if (!newstring)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		newstring[j++] = s1[i++];
	i = 0;
	while (s2[i])
		newstring[j++] = s2[i++];
	i = 0;
	while (s3[i])
		newstring[j++] = s3[i++];
	newstring[j] = '\0';
	return (newstring);
}

char	*get_path_from_env(t_data *minishell)
{
	t_env	*tmp;

	if (!minishell->env)
		return (NULL);
	tmp = minishell->env;
	while (tmp && ft_strcmp(tmp->var_name, "PATH") != 0)
		tmp = tmp->next;
	if (tmp == NULL)
		return (NULL);
	else
		return (tmp->var_value);
}
