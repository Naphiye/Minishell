/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_absolute_relative_path.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 17:45:25 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/25 15:09:27 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	check_absolut_or_relative_path(t_cmd *cmd, t_data *minishell, char *path)
{
	int	error_code;

	if (is_directory(path))
		exit(error(IS_A_DIRECTORY_CMD, path, cmd, minishell));
	if (access(path, F_OK | X_OK) == -1)
	{
		error_code = errno;
		exit(error(error_code, cmd->cmd_name, cmd, minishell));
	}
	else
		return (1);
}

void	create_absolute_relative_cmd_path(t_cmd *cmd, t_data *minishell)
{
	cmd->cmd_path = ft_substr(cmd->cmd_name, 0, ft_strlen(cmd->cmd_name));
	if (cmd->cmd_path == NULL)
		exit(error(MALLOC_ERROR, cmd->cmd_name, cmd, minishell));
}
