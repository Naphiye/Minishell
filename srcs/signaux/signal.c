/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:35:29 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 16:09:04 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

extern volatile sig_atomic_t	g_signal_flag;

static void	signal_sig_int(void)
{
	if (g_signal_flag == IN_HEREDOC_PROMPT)
	{
		write(1, "\n", 2);
		close(STDIN_FILENO);
		g_signal_flag = HEREDOC_STOP;
	}
	else if (g_signal_flag != NOT_IN_CHILDREN
		&& g_signal_flag != IN_HEREDOC_PROMPT && g_signal_flag != CTRL_C)
	{
		write(1, "\n", 2);
		rl_redisplay();
		g_signal_flag = CTRL_C;
	}
	else if (g_signal_flag == NOT_IN_CHILDREN || g_signal_flag == CTRL_C)
	{
		write(1, "\n", 2);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_flag = CTRL_C;
	}
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		signal_sig_int();
	}
	else if (signum == SIGQUIT)
	{
		if (g_signal_flag != NOT_IN_CHILDREN
			&& g_signal_flag != IN_HEREDOC_PROMPT && g_signal_flag != CTRL_C)
		{
			printf("Quit (core dumped)\n");
			rl_on_new_line();
			g_signal_flag = CTRL_BACKSLASH;
		}
		else
		{
			printf("\033[2D\033[0K");
			rl_on_new_line();
			rl_redisplay();
		}
	}
	return ;
}

void	listen_signal(t_data *minishell)
{
	if (minishell->in_children == NOT_IN_CHILDREN)
	{
		signal(SIGQUIT, signal_handler);
		signal(SIGINT, signal_handler);
	}
	return ;
}
