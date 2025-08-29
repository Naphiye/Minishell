/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:37:43 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:59:02 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

extern volatile sig_atomic_t	g_signal_flag;

void	stop_here_doc(char *delimiter, int i, t_data *minishell, int *error)
{
	char	*number_i;

	*error = 1;
	if (g_signal_flag != HEREDOC_STOP)
	{
		i++;
		number_i = ft_itoa(i);
		ft_putstr_fd("bash: warning: here-document at line ", STDERR_FILENO);
		if (number_i)
		{
			ft_putstr_fd(number_i, STDERR_FILENO);
			free(number_i);
		}
		else
			ft_putstr_fd("'malloc error'", STDERR_FILENO);
		ft_putstr_fd(" delimited by end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd(delimiter, STDERR_FILENO);
		ft_putstr_fd("')\n", STDERR_FILENO);
		minishell->exit_status = 0;
		g_signal_flag = NOT_IN_CHILDREN;
	}
}

static void	put_char_error(char *token)
{
	ft_putchar_fd(token[0], STDERR_FILENO);
	if (token[1] && (token[1] == '>' || token[1] == '<'))
		ft_putchar_fd(token[1], STDERR_FILENO);
}

void	error_parsing(int flag, char *token, t_data *minishell)
{
	if (flag == SYNTAX_ERROR)
	{
		ft_putstr_fd("error bash: syntax error near unexpected token `",
			STDERR_FILENO);
		if (token && token[0])
			put_char_error(token);
		else
			ft_putstr_fd("newline", STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	else if (flag == HERE_STRING_ERROR)
	{
		ft_putstr_fd("error: This shell does not handle here_string\n",
			STDERR_FILENO);
	}
	else if (flag == UNCLOSED_QUOTES)
	{
		ft_putstr_fd("error: This shell does not interpret", STDERR_FILENO);
		ft_putstr_fd(" input with unclosed quotes\n", STDERR_FILENO);
	}
	minishell->exit_status = 2;
	return ;
}
