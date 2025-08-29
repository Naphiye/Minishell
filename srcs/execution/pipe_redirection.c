/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 12:41:04 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/19 09:31:03 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	pipe_redirection(t_cmd *cmd, t_data *minishell)
{
	if (cmd->cmd_pos == ONLY_ONE_CMD)
		only_one_cmd_case_pipe_redirection(cmd);
	else if (cmd->cmd_pos == FIRST_CMD)
		first_cmd_case_pipe_redirection(cmd, minishell);
	else if (cmd->cmd_pos == BETWEEN_CMD)
		between_cmd_case_pipe_redirection(cmd, minishell);
	else if (cmd->cmd_pos == LAST_CMD)
		last_cmd_case_pipe_redirection(cmd, minishell);
}

void	only_one_cmd_case_pipe_redirection(t_cmd *cmd)
{
	close(cmd->pipe[0]);
	close(cmd->pipe[1]);
}

void	first_cmd_case_pipe_redirection(t_cmd *cmd, t_data *minishell)
{
	close(cmd->pipe[0]);
	if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
		error_fct(DUP2_FAIL, cmd->cmd_name, cmd, minishell);
	close(cmd->pipe[1]);
}

void	between_cmd_case_pipe_redirection(t_cmd *cmd, t_data *minishell)
{
	close(cmd->pipe[0]);
	if (dup2(cmd->last_pipe_read_end, STDIN_FILENO) == -1)
		error_fct(DUP2_FAIL, cmd->cmd_name, cmd, minishell);
	close(cmd->last_pipe_read_end);
	if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
		error_fct(DUP2_FAIL, cmd->cmd_name, cmd, minishell);
	close(cmd->pipe[1]);
}

void	last_cmd_case_pipe_redirection(t_cmd *cmd, t_data *minishell)
{
	close(cmd->pipe[1]);
	if (dup2(cmd->last_pipe_read_end, STDIN_FILENO) == -1)
		error_fct(DUP2_FAIL, cmd->cmd_name, cmd, minishell);
	close(cmd->pipe[0]);
	close(cmd->last_pipe_read_end);
}
