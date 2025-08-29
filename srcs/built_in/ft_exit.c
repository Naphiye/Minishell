/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 12:37:03 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/23 18:07:16 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

// Je te change lexit status de la commande
// dans lenfant tu devras le recuperer dans le parent
// Si je return exit success il faut quitter
// le programme si cest failure non

int	exit_with_args(char **args, t_data *minishell, long long *arg_exit)
{
	int	exit_status;

	exit_status = 0;
	if (!it_is_only_digit(args[1]) || there_is_an_overflow(args[1], arg_exit))
	{
		minishell->exit_status = 2;
		if (minishell->cmd_list->cmd_pos == ONLY_ONE_CMD)
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		ft_print_exit_error(2, args[1]);
		exit_status = minishell->exit_status;
		clean_all(minishell);
		exit(exit_status);
	}
	else if (args[2])
		return (ft_print_exit_error(EXIT_FAILURE, args[1]), EXIT_FAILURE);
	else
	{
		minishell->exit_status = *arg_exit % 256;
		if (minishell->cmd_list->cmd_pos == ONLY_ONE_CMD)
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit_status = minishell->exit_status;
		clean_all(minishell);
		exit(exit_status);
	}
}

int	ft_exit(char **args, t_data *minishell)
{
	long long	arg_exit;

	arg_exit = 0;
	if (args[1])
		return (exit_with_args(args, minishell, &arg_exit));
	else
	{
		if (minishell->cmd_list->cmd_pos == ONLY_ONE_CMD)
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		clean_all(minishell);
		exit(EXIT_SUCCESS);
	}
}

// int main (int argc, char **argv)
// {
// 	t_data 	data;

// 	(void)argc;
// 	ft_exit(argv, &data);
// 	unit_test(&data);
// 	return (1);
// }