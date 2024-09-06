/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:46:30 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 13:23:52 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"

extern volatile sig_atomic_t	g_signal_flag;

static char	*get_input(t_data *minishell)
{
	static char	prompt[23] = "Fanny & Antoine SHELL :";
	char		*input;

	input = readline(prompt);
	if (input == NULL)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		clean_all(minishell);
		exit(EXIT_SUCCESS);
	}
	if (*input)
		add_history(input);
	if (g_signal_flag == CTRL_C)
		minishell->exit_status = 130;
	return (input);
}

bool	is_only_one_built_in(t_data *minishell)
{
	if (minishell->cmd_list->cmd_type == BUILT_IN
		&& minishell->cmd_list->cmd_pos == ONLY_ONE_CMD)
		return (true);
	return (false);
}

static void	re_init_shell_for_new_input(t_data *minishell)
{
	lst_cmd_clear(minishell->cmd_list);
	minishell->cmd_list = NULL;
	minishell->error = 0;
	if (minishell->save_stdin_heredoc != -1)
		close(minishell->save_stdin_heredoc);
	minishell->save_stdin_heredoc = -1;
	minishell->save_stdin_heredoc = dup(STDIN_FILENO);
	g_signal_flag = NOT_IN_CHILDREN;
	return ;
}

static int	exec_only_one_built_in(t_data *minishell)
{
	int	ret;

	ret = exec_built_in(minishell->cmd_list, minishell);
	plug_back_stdin_stdout(minishell);
	close_fd(minishell->cmd_list);
	return (ret);
}

void	launch_minishell(t_data *minishell)
{
	char	**tokens;
	char	*input;

	tokens = NULL;
	input = NULL;
	while (1)
	{
		input = get_input(minishell);
		if (!input)
			malloc_error(minishell, NULL, NULL);
		if (!is_this_empty_line(input))
		{
			do_parsing(minishell, tokens, input);
			if (!minishell->error && is_only_one_built_in(minishell))
				minishell->exit_status = exec_only_one_built_in(minishell);
			else if (!minishell->error)
				minishell->exit_status = execution(minishell);
		}
		else
		{
			minishell->exit_status = 0;
			free(input);
		}
		re_init_shell_for_new_input(minishell);
	}
}
