/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 17:17:48 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:29:53 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

bool	is_this_empty_line(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (false);
		i++;
	}
	return (true);
}

static bool	heredoc_error(t_data *minishell, char *input, int i, int *error)
{
	if (i)
		error_parsing(SYNTAX_ERROR, &input[i], minishell);
	else
		error_parsing(HERE_STRING_ERROR, input, minishell);
	*error = 1;
	return (false);
}

static bool	is_there_here_doc(char *line, t_data *minishell, int *error)
{
	int				i;
	t_into_quote	in;

	ft_bzero(&in, sizeof(t_into_quote));
	i = 0;
	while (line[i])
	{
		change_quote_flag(&in, line[i]);
		if (line[i] == '<' && line[i + 1] == '<' && not_in_quote(&in))
		{
			if (line[i + 2] == '<')
				return (heredoc_error(minishell, line, 0, error));
			else
			{
				while (line[i + 2] && ft_isspace(line[i + 2]))
					i++;
				if (line[i + 2] && (line[i + 2] == '>' || line[i + 2] == '<'))
					return (heredoc_error(minishell, line, i + 2, error));
				else if (line[i + 2])
					return (true);
			}
		}
		i++;
	}
	return (false);
}

static void	init_cmd_pos(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	tmp = cmd_list;
	while (tmp)
	{
		if (tmp->prev == NULL && tmp->next == NULL && tmp->cmd_name != NULL)
			tmp->cmd_pos = ONLY_ONE_CMD;
		else if (tmp->prev == NULL && tmp->next != NULL)
			tmp->cmd_pos = FIRST_CMD;
		else if (tmp->prev != NULL && tmp->next != NULL)
			tmp->cmd_pos = BETWEEN_CMD;
		else if (tmp->prev != NULL && tmp->next == NULL)
			tmp->cmd_pos = LAST_CMD;
		else
			tmp->cmd_pos = NO_CMD;
		tmp = tmp->next;
	}
}

void	do_parsing(t_data *minishell, char **tokens, char *input)
{
	if (is_there_unclosed_quote(input))
	{
		error_parsing(UNCLOSED_QUOTES, input, minishell);
		error_detected(&minishell->error);
		return ;
	}
	if (is_there_here_doc(input, minishell, &minishell->error))
		input = process_heredocs(input, minishell);
	if (minishell->save_stdin_heredoc != -1)
		close(minishell->save_stdin_heredoc);
	if (!minishell->error)
	{
		tokens = tokenise_input(input, minishell);
		if (minishell->error)
		{
			ft_free_split(tokens);
			return ;
		}
		tokens = handle_expand(tokens, minishell);
		init_cmd_list(tokens, minishell);
		ft_free_split(tokens);
		init_cmd_pos(minishell->cmd_list);
	}
	free(input);
	return ;
}
