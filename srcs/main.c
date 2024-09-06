/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:58:34 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 16:00:36 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/built_in.h"
#include "../headers/minishell.h"
#include "../headers/parsing.h"

volatile sig_atomic_t	g_signal_flag = 0;

int	main(int argc, char **argv, char *envp[])
{
	t_data	*minishell;

	if (!isatty(STDOUT_FILENO) || argv[1] != NULL)
	{
		ft_putstr_fd("This shell does not handle arguments ", STDERR_FILENO);
		ft_putstr_fd("and cannot be redirected.\n", STDERR_FILENO);
		return (0);
	}
	(void)argc;
	minishell = NULL;
	minishell = ft_init_data(envp, minishell);
	listen_signal(minishell);
	launch_minishell(minishell);
	return (0);
}
