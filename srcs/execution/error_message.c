/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 09:20:51 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/25 15:08:37 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	print_err_msg_2(int error)
{
	if (error == ENOMEM)
		return (ft_putstr_fd(": Out of memory\n", STDERR_FILENO), 1);
	else if (error == EIO)
		return ((ft_putstr_fd(": I/O error\n\n", STDERR_FILENO)), 1);
	else if (error == EROFS)
		return (ft_putstr_fd(": Read-only file system\n", STDERR_FILENO), 1);
	else if (error == EFAULT)
		return (ft_putstr_fd(": Bad address\n", STDERR_FILENO), 1);
	else if (error == ELOOP)
		return (ft_putstr_fd(": Too many levels of symbolic links\n",
				STDERR_FILENO), 1);
	else if (error == ENAMETOOLONG)
		return (ft_putstr_fd(": File name too long\n", STDERR_FILENO), 1);
	else if (error == IS_A_DIRECTORY)
		return (ft_putstr_fd(": Is a directory\n", STDERR_FILENO), 1);
	else if (error == IS_A_DIRECTORY_CMD)
		return (ft_putstr_fd(": Is a directory\n", STDERR_FILENO), 126);
	else
		ft_putstr_fd("Unknown error\n", STDERR_FILENO);
	return (1);
}

int	print_err_msg(int error)
{
	if (error == EACCES)
		return (ft_putstr_fd(": Permission denied\n", STDERR_FILENO), 126);
	else if (error == PERMISSION_DENIED)
		return (ft_putstr_fd(": Permission denied\n", STDERR_FILENO), 1);
	else if (error == ENOENT)
		return (ft_putstr_fd(": No such file or directory\n", STDERR_FILENO),
			127);
	else if (error == NO_SUCH_FILE)
		return (ft_putstr_fd(": No such file or directory\n", STDERR_FILENO),
			1);
	else if (error == ENOTDIR)
		return (ft_putstr_fd(": Not a directory\n", STDERR_FILENO), 126);
	else if (error == EIO)
		return (ft_putstr_fd(": Input/output error\n", STDERR_FILENO), 1);
	else if (error == HOME_NOT_SET)
		return (ft_putstr_fd(": HOME not set\n", STDERR_FILENO), 1);
	else if (error == TOO_MANY_ARGUMENTS)
		return (ft_putstr_fd(": too many arguments\n", STDERR_FILENO), 1);
	else if (error == CMD_NOT_FOUND)
		return (ft_putstr_fd(": command not found\n", STDERR_FILENO), 127);
	else
		return (print_err_msg_2(error));
	return (1);
}

int	print_err_msg_function(int error)
{
	if (error == DUP2_FAIL)
		return (ft_putstr_fd(": DUP2 FAIL\n", 2), 1);
	if (error == DUP_FAIL)
		return (ft_putstr_fd(": DUP FAIL\n", 2), 1);
	else if (error == OPEN_FAIL)
		return (ft_putstr_fd(": OPEN FAIL\n", 2), 1);
	else if (error == PIPE_FAIL)
		return (ft_putstr_fd(": PIPE_FAIL\n", 2), 1);
	else if (error == FORK_FAIL)
		return (ft_putstr_fd(": FORK_FAIL\n", 2), 1);
	else if (error == MALLOC_ERROR)
		return (ft_putstr_fd(": MALLOC_ERROR\n", 2), 1);
	if (error == NO_SPC)
		return (ft_putstr_fd(": write error: No space left on device\n", 2), 1);
	return (1);
}
