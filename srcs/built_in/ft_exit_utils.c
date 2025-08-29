/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:56:21 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/23 18:10:54 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/built_in.h"

bool	ft_atoll(const char *arg, long long *arg_exit)
{
	int	i;

	i = 0;
	if (!ft_strcmp(arg, "-9223372036854775808"))
	{
		*arg_exit = -9223372036854775807LL - 1;
		return (true);
	}
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i] != 0 && arg[i] > 47 && arg[i] < 58)
	{
		if (*arg_exit > (LLONG_MAX / 10) || (*arg_exit == (LLONG_MAX / 10)
				&& (arg[i] - '0') > LLONG_MAX % 10))
			return (false);
		*arg_exit = *arg_exit * 10 + (arg[i] - '0');
		i++;
	}
	if (arg[0] == '-')
		*arg_exit = -(*arg_exit);
	return (true);
}

int	it_is_only_digit(char *arg)
{
	int	i;

	i = 0;
	if (arg && (arg[0] == '-' || arg[0] == '+'))
		i++;
	while (arg && arg[i])
	{
		if (arg[i] < 48 || arg[i] > 57)
			return (0);
		i++;
	}
	return (1);
}

int	there_is_an_overflow(char *arg, long long *arg_exit)
{
	if (!ft_atoll(arg, arg_exit))
	{
		*arg_exit = 0;
		return (1);
	}
	else
		return (0);
}

void	ft_print_exit_error(int error_num, char *args)
{
	if (error_num == 1)
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
	else if (error_num == 2)
	{
		ft_putstr_fd("bash: exit: ", STDERR_FILENO);
		ft_putstr_fd(args, STDERR_FILENO);
		ft_putstr_fd (": numeric argument required\n", STDERR_FILENO);
	}
}
