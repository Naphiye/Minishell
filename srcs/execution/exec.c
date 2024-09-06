/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:16:05 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/27 13:16:12 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

extern volatile sig_atomic_t	g_signal_flag;

int	execution(t_data *minishell)
{
	int		pid;
	t_cmd	*cmd;
	int		exit_status;

	exit_status = 0;
	cmd = minishell->cmd_list;
	g_signal_flag = NOT_IN_CHILDREN;
	while (cmd != NULL)
	{
		if (pipe(cmd->pipe) == -1)
			exit(error_fct(PIPE_FAIL, cmd->cmd_name, cmd, minishell));
		pid = fork();
		if (pid == -1)
			exit(error_fct(FORK_FAIL, cmd->cmd_name, cmd, minishell));
		if (pid == 0)
			child_process(cmd, minishell);
		g_signal_flag = 0;
		clean_parent_fd_and_set_last_pipe_read_end(cmd);
		cmd = cmd->next;
	}
	return (wait_for_children_to_end(exit_status, pid));
}

void	child_process(t_cmd *cmd, t_data *minishell)
{
	int	exit_status;

	minishell->in_children = IN_CHILDREN;
	pipe_redirection(cmd, minishell);
	operand_redirection(cmd, minishell);
	if (minishell->error)
	{
		exit_status = minishell->exit_status;
		clean_all(minishell);
		exit(exit_status);
	}
	exec_cmd(cmd, minishell, &minishell->exit_status);
	close_fd(cmd);
	exit_status = minishell->exit_status;
	clean_all(minishell);
	exit(exit_status);
}

void	clean_parent_fd_and_set_last_pipe_read_end(t_cmd *cmd)
{
	if (cmd->next != NULL)
		cmd->next->last_pipe_read_end = cmd->pipe[0];
	close(cmd->pipe[1]);
	if (cmd->prev != NULL)
		close(cmd->prev->pipe[0]);
	if (cmd->next == NULL)
		close(cmd->pipe[0]);
}

void	exec_cmd(t_cmd *cmd, t_data *minishell, int *exit_status)
{
	if (cmd->cmd_type == CMD)
	{
		if (get_path_from_env(minishell) == NULL && ft_strchr(cmd->cmd_name,
				'/') == NULL)
		{
			cmd->cmd_path = ft_strjoin("/", cmd->cmd_name);
			if (cmd->cmd_path == NULL)
				exit(error_fct(MALLOC_ERROR, cmd->cmd_name, cmd, minishell));
			check_absolut_or_relative_path(cmd, minishell, cmd->cmd_path);
		}
		if (ft_strchr(cmd->cmd_name, '/'))
		{
			if (check_absolut_or_relative_path(cmd, minishell, cmd->cmd_name))
				create_absolute_relative_cmd_path(cmd, minishell);
		}
		else
			find_cmd_path_with_path(cmd, minishell);
		if (cmd->cmd_path == NULL)
			exit(error(CMD_NOT_FOUND, cmd->cmd_name, cmd, minishell));
		execve(cmd->cmd_path, cmd->cmd_args, minishell->envp);
	}
	else if (cmd->cmd_type == BUILT_IN)
		*exit_status = exec_built_in(cmd, minishell);
}

int	wait_for_children_to_end(int exit_status, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	while (wait(NULL) != -1)
		continue ;
	if (g_signal_flag == CTRL_C)
		return (130);
	else if (g_signal_flag == CTRL_BACKSLASH)
		return (131);
	return (exit_status);
}
/*
int	wait_for_children_to_end(int exit_status, t_data *minishell, int pid)
{
	int		status;
	int		i;
	int		cmd_num;
	t_cmd	*tmp;

	cmd_num = 0;
	tmp = minishell->cmd_list;
	while (tmp)
	{
		cmd_num++;
		tmp = tmp->next;
	}
	i = 0;
	while (i < cmd_num)
	{
		while (wait(NULL) == -1)
			continue ;
		i++;
		if (i == cmd_num)
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
		}
	}
	printf("exit status : %d\n", exit_status);
	return (exit_status);
}*/