/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:14:13 by anastruc          #+#    #+#             */
/*   Updated: 2024/09/06 11:09:37 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "parsing.h"

void	get_next_line_and_init_envp_error(int fd, t_data *minishell,
		char *etc_line)
{
	close(fd);
	free(etc_line);
	malloc_error(minishell, NULL, NULL);
}

t_env	*init_node_env(void *var_name, void *var_value)
{
	t_env	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->var_name = var_name;
	new->var_value = var_value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	ft_lstenvadd_back(t_env **env_lst, t_env *new_node)
{
	t_env	*current;

	if (env_lst)
	{
		if (*env_lst)
		{
			current = *env_lst;
			while (current->next)
			{
				current = current->next;
			}
			current->next = new_node;
			new_node->prev = current;
		}
		else
		{
			if (new_node == NULL)
				*env_lst = NULL;
			else
				*env_lst = new_node;
		}
	}
}

char	*create_shlvl(t_data *minishell, char *etc_line)
{
	etc_line = malloc(sizeof(char) * 8);
	if (!etc_line)
		malloc_error(minishell, NULL, NULL);
	ft_strlcpy(etc_line, "SHLVL=1", 8);
	return (etc_line);
}

t_env	*create_env_with_etc_file(t_env *new, t_data *minishell, int fd,
		t_env *lst_env)
{
	int		i;
	char	*etc_line;

	etc_line = NULL;
	while (1)
	{
		if (etc_line)
			free(etc_line);
		etc_line = gnl_and_init_envp(fd, minishell, &minishell->partial_env);
		i = 0;
		if (etc_line == NULL)
			break ;
		while (etc_line[i] != '\0' && etc_line[i] != '=')
			i++;
		new = create_new_node(i, etc_line, 1);
		if (new == NULL)
			malloc_error_env(lst_env, fd, minishell);
		ft_lstenvadd_back(&lst_env, new);
		if (ft_strncmp(etc_line, "SHLVL=", 6) == 0)
		{
			free(etc_line);
			break ;
		}
	}
	return (lst_env);
}
