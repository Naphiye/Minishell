/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_redir_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:19:26 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/09 14:53:49 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

// typedef struct s_redir_file
// {
// 	char					*file_name;
// 	int						redir_type;
// 	struct s_redir_file		*next;
// }							t_redir_file;

t_redir_file	*lst_redir_file_new_node(void)
{
	t_redir_file	*new;

	new = ft_calloc(sizeof(t_redir_file), 1);
	if (!new)
		return (NULL);
	new->file_name = NULL;
	new->redir_type = -1;
	return (new);
}

t_redir_file	*lst_redir_file_last(t_redir_file *file_lst)
{
	t_redir_file	*tmp;

	if (!file_lst)
		return (NULL);
	tmp = file_lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	lst_redir_file_addback(t_redir_file **file_lst, t_redir_file *new_node)
{
	t_redir_file	*last;

	if (!file_lst || !new_node)
		return ;
	if (!*file_lst)
		*file_lst = new_node;
	else
	{
		last = lst_redir_file_last(*file_lst);
		last->next = new_node;
	}
}

void	lst_redir_file_clear(t_redir_file *file_lst)
{
	t_redir_file	*tmp;

	while (file_lst)
	{
		tmp = file_lst;
		if (tmp->file_name)
			free(tmp->file_name);
		file_lst = file_lst->next;
		free(tmp);
	}
}
