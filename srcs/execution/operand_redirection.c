/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operand_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:16:57 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/25 17:24:44 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static void	save_std(t_cmd *cmd, t_data *minishell, int *save_std, int flag_std)
{
	if (flag_std)
	{
		*save_std = dup(STDOUT_FILENO);
		if (*save_std == -1)
			exit(error_fct(DUP_FAIL, "save_stdout", cmd, minishell));
	}
	else
	{
		*save_std = dup(STDIN_FILENO);
		if (*save_std == -1)
			exit(error_fct(DUP_FAIL, "save_stdin", cmd, minishell));
	}
}

void	operand_redirection(t_cmd *cmd, t_data *minishell)
{
	check_operand_file(cmd, minishell);
	if (minishell->error)
		return ;
	if (cmd->infilefd != -1)
	{
		if (is_only_one_built_in(minishell))
			save_std(cmd, minishell, &minishell->cmd_list->save_stdin, 0);
		if (dup2(cmd->infilefd, STDIN_FILENO) == -1)
			exit(error_fct(DUP2_FAIL, "last infile", cmd, minishell));
		close(cmd->infilefd);
	}
	if (cmd->outfilefd != -1)
	{
		if (is_only_one_built_in(minishell))
			save_std(cmd, minishell, &minishell->cmd_list->save_stdout, 1);
		if (dup2(cmd->outfilefd, STDOUT_FILENO) == -1)
			exit(error_fct(DUP2_FAIL, "last outfile", cmd, minishell));
		close(cmd->outfilefd);
	}
}

void	check_operand_file(t_cmd *cmd, t_data *minishell)
{
	t_redir_file	*tmp_file;

	if (cmd->file)
	{
		tmp_file = cmd->file;
		while (tmp_file)
		{
			if (tmp_file->redir_type == APPEND
				|| tmp_file->redir_type == DST_REDIR)
				check_file_out(cmd, minishell, tmp_file);
			else if (tmp_file->redir_type == HERE_DOC
				|| tmp_file->redir_type == SRC_REDIR)
				check_file_in(cmd, minishell, tmp_file);
			tmp_file = tmp_file->next;
		}
	}
}

void	check_file_in(t_cmd *cmd, t_data *minishell, t_redir_file *tmp_file_in)
{
	static int	err_code = 0;

	if (err_code)
		return ;
	errno = 0;
	check_dir(tmp_file_in->file_name, cmd, minishell, 0);
	if (access(tmp_file_in->file_name, F_OK | R_OK) == 0)
		file_in_existing(tmp_file_in, cmd, minishell);
	else
	{
		err_code = errno;
		file_in_unexist_or_no_acces(tmp_file_in, cmd, minishell, err_code);
	}
}

void	check_file_out(t_cmd *cmd, t_data *minishell,
		t_redir_file *tmp_file_out)
{
	int	err_code;

	errno = 0;
	if (minishell->error)
		return ;
	if (check_dir(tmp_file_out->file_name, cmd, minishell, 1) != 0)
		return ;
	if (is_directory(tmp_file_out->file_name))
	{
		error(IS_A_DIRECTORY, tmp_file_out->file_name, cmd, minishell);
		return ;
	}
	if (access(tmp_file_out->file_name, F_OK | W_OK) == 0)
	{
		err_code = errno;
		file_out_existing(tmp_file_out, cmd, minishell);
	}
	else
	{
		err_code = errno;
		file_out_unexist_or_no_acces(tmp_file_out, cmd, minishell, err_code);
	}
	if (tmp_file_out->pos != LAST_FILE && cmd->outfilefd != -1)
		close(cmd->outfilefd);
}
