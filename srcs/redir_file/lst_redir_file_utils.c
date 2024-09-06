/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_redir_file_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 11:50:44 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/09 14:54:08 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_redir_file	*init_node_lst_file(int redirtype, char *filename)
{
	t_redir_file	*new;

	new = ft_calloc(sizeof(t_redir_file), 1);
	if (!new)
		return (NULL);
	new->file_name = filename;
	new->redir_type = redirtype;
	new->next = NULL;
	return (new);
}
