/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:08:34 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 15:10:16 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

void	set_last_flag(int *file_pos)
{
	*file_pos = LAST_FILE;
	return ;
}

void	init_pos_file(t_cmd *cmd)
{
	t_redir_file	*last_out;
	t_redir_file	*last_in;
	t_redir_file	*tmp_file;

	last_out = NULL;
	last_in = NULL;
	if (cmd->file)
	{
		tmp_file = cmd->file;
		while (tmp_file)
		{
			if (tmp_file->redir_type == APPEND
				|| tmp_file->redir_type == DST_REDIR)
				last_out = tmp_file;
			else if (tmp_file->redir_type == HERE_DOC
				|| tmp_file->redir_type == SRC_REDIR)
				last_in = tmp_file;
			tmp_file = tmp_file->next;
		}
		if (last_out)
			set_last_flag(&last_out->pos);
		if (last_in)
			set_last_flag(&last_in->pos);
	}
}
