/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 14:11:03 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:23:05 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

static void	check_three_redir_sign(char *line, int *index, t_data *minishell,
		int *error)
{
	int	i;

	i = 2;
	while (line[*index + i] && ft_isspace(line[*index + i]))
		i++;
	if (line[*index + i] == '>' || line[*index + i] == '<')
	{
		error_parsing(SYNTAX_ERROR, &line[*index + i], minishell);
		*error = 1;
	}
	else
		*index += 2;
}

static void	inversed_redir_sign(char *line, int index, t_data *minishell,
		int *error)
{
	error_parsing(SYNTAX_ERROR, &line[index], minishell);
	*error = 1;
}

static void	is_redir_part_2(t_data *minishell, int *index, char *line, int *i)
{
	char	redir;
	char	redir_2;

	redir = line[*index];
	if (redir == '<')
		redir_2 = '>';
	else
		redir_2 = '<';
	while (ft_isspace(line[*index + *i]))
		*i += 1;
	if (!line[*index + *i])
		inversed_redir_sign(line, *index + *i, minishell, &(minishell->error));
	else if (is_separator_char_without_quote(line[*index + *i]))
		inversed_redir_sign(line, *index + *i, minishell, &(minishell->error));
	*index += 1;
}

bool	is_redir(char *line, int *index, t_data *minishell)
{
	char	redir;
	char	redir_2;
	int		i;

	i = 1;
	redir = line[*index];
	if (redir == '<' || redir == '>')
	{
		if (redir == '<')
			redir_2 = '>';
		else
			redir_2 = '<';
		if (line[*index + 1] == redir)
			check_three_redir_sign(line, index, minishell, &(minishell->error));
		else if (ft_isspace(line[*index + i]) || line[*index + i] == redir_2
			|| line[*index + i] == '|')
			is_redir_part_2(minishell, index, line, &i);
		else
		{
			*index += 1;
			return (true);
		}
		return (true);
	}
	return (false);
}
