/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 16:48:10 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/25 15:09:16 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

int	exec_built_in(t_cmd *cmd, t_data *minishell)
{
	if (cmd->cmd_pos == ONLY_ONE_CMD)
		operand_redirection(cmd, minishell);
	if (minishell->error)
		return (minishell->exit_status);
	if (ft_strcmp(cmd->cmd_name, "echo") == 0)
		return (ft_echo(cmd->cmd_args, minishell));
	else if (ft_strcmp(cmd->cmd_name, "cd") == 0)
		return (ft_cd(cmd, minishell));
	else if (ft_strcmp(cmd->cmd_name, "pwd") == 0)
		return (ft_pwd(cmd));
	else if (ft_strcmp(cmd->cmd_name, "export") == 0)
		return (ft_export(cmd->cmd_args, minishell));
	else if (ft_strcmp(cmd->cmd_name, "unset") == 0)
		return (ft_unset(cmd->cmd_args, minishell));
	else if (ft_strcmp(cmd->cmd_name, "env") == 0)
		return (ft_env(minishell));
	else if (ft_strcmp(cmd->cmd_name, "exit") == 0)
		return (ft_exit(cmd->cmd_args, minishell));
	return (0);
}
