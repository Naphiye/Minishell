/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_and_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:29:56 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/25 17:17:41 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	close_fd(t_cmd *cmd)
{
	t_redir_file	*tmp;

	if (cmd->pipe[0] != -1)
		close(cmd->pipe[0]);
	if (cmd->pipe[1] != -1)
		close(cmd->pipe[1]);
	if (cmd->last_pipe_read_end != -1)
		close(cmd->last_pipe_read_end);
	if (cmd->infilefd != -1)
		close(cmd->infilefd);
	if (cmd->outfilefd != -1)
		close(cmd->outfilefd);
	if (cmd->file)
	{
		tmp = cmd->file;
		while (tmp)
		{
			if (tmp->redir_type == HERE_DOC)
				unlink(tmp->file_name);
			tmp = tmp->next;
		}
	}
}

/*last part : Unlink all here_doc even if the first infile to be check cannot
 be acces or doesn't exist,which lead the program to quit.
 We need to unlink all the here_doc file created in the parsing*/
int	error_fct(int err_type, char *elmt, t_cmd *cmd, t_data *minishell)
{
	int	err_code_shell;

	error_detected(&minishell->error);
	if (cmd)
		close_fd(cmd);
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(elmt, 2);
	err_code_shell = print_err_msg_function(err_type);
	minishell->exit_status = err_code_shell;
	if (err_type != DUP_FAIL && err_type != DUP2_FAIL
		&& is_only_one_built_in(minishell) && err_type != MALLOC_ERROR)
		return (err_code_shell);
	else
	{
		clean_all(minishell);
		exit(err_code_shell);
	}
}

int	error(int err_type, char *elmt, t_cmd *cmd, t_data *minishell)
{
	int	err_code_shell;

	error_detected(&minishell->error);
	if (cmd)
		close_fd(cmd);
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(elmt, 2);
	err_code_shell = print_err_msg(err_type);
	minishell->exit_status = err_code_shell;
	if (is_only_one_built_in(minishell) && err_type != MALLOC_ERROR)
		return (err_code_shell);
	else
	{
		clean_all(minishell);
		exit(err_code_shell);
	}
}

int	error_chdir(int err_code, char *path, t_cmd *cmd)
{
	if (cmd)
		close_fd(cmd);
	ft_putstr_fd("bash: cd", STDERR_FILENO);
	if (path)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
	}
	print_err_msg(err_code);
	return (1);
}

void	plug_back_stdin_stdout(t_data *minishell)
{
	if (minishell->cmd_list->save_stdin != -1)
	{
		if (dup2(minishell->cmd_list->save_stdin, STDIN_FILENO) == -1)
			exit(error_fct(DUP2_FAIL, "save_stdin", minishell->cmd_list,
					minishell));
		close(minishell->cmd_list->save_stdin);
	}
	if (minishell->cmd_list->save_stdout != -1)
	{
		if (dup2(minishell->cmd_list->save_stdout, STDOUT_FILENO) == -1)
			exit(error_fct(DUP2_FAIL, "save_stdout", minishell->cmd_list,
					minishell));
		close(minishell->cmd_list->save_stdout);
	}
}
