/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:13:09 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/19 17:16:35 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

static int	error_pwd(int err_code, t_cmd *cmd)
{
	if (cmd)
		close_fd(cmd);
	ft_putstr_fd("pwd: error retrieving current directory: getcwd",
		STDOUT_FILENO);
	ft_putstr_fd(": cannot access parent directories", STDOUT_FILENO);
	print_err_msg(err_code);
	return (1);
}

int	ft_pwd(t_cmd *cmd)
{
	int		err_code;
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		err_code = errno;
		return (error_pwd(err_code, cmd));
	}
	else
	{
		printf("%s\n", path);
		free(path);
		return (0);
	}
}

// int main (int argc, char **argv)
// {
//     if (argc >= 2)
//     {
//         if (strcmp(argv[1], "pwd") == 0)
//         {
//             printf("BUILT_IN PWD is called\n");
//             return(ft_pwd());
//         }
//     }
//     else
//         return(1);
// }