/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:00:23 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 12:04:02 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_cmd	*lst_cmd_new_node(void)
{
	t_cmd	*new;

	new = ft_calloc(sizeof(t_cmd), 1);
	if (!new)
		return (NULL);
	new->cmd_type = -1;
	new->cmd_pos = -1;
	new->infilefd = -1;
	new->outfilefd = -1;
	new->last_pipe_read_end = -1;
	new->pipe[0] = -1;
	new->pipe[1] = -1;
	new->save_stdin = -1;
	new->save_stdout = -1;
	return (new);
}

t_cmd	*lst_cmd_last(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	if (!cmd_list)
		return (NULL);
	tmp = cmd_list;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	lst_cmd_addback(t_cmd **cmd_list, t_cmd *new_node)
{
	t_cmd	*last;

	if (!cmd_list || !new_node)
		return ;
	if (!*cmd_list)
		*cmd_list = new_node;
	else
	{
		last = lst_cmd_last(*cmd_list);
		last->next = new_node;
		new_node->prev = last;
	}
}

void	lst_cmd_clear(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		if (tmp->cmd_name)
			free(tmp->cmd_name);
		if (tmp->cmd_args)
			ft_free_split(tmp->cmd_args);
		if (tmp->cmd_path)
			free(tmp->cmd_path);
		lst_redir_file_clear(tmp->file);
		free(tmp);
	}
}
