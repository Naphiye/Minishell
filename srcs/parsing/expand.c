/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:11:51 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 12:55:41 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

bool	there_is_expand(char *line)
{
	int				i;
	t_into_quote	into;

	into.simple_quote = 0;
	into.double_quote = 0;
	i = 0;
	while (line[i])
	{
		change_quote_flag(&into, line[i]);
		if (line[i] == '$' && (!into.simple_quote))
			break ;
		i++;
	}
	if (line[i] == '$')
		i++;
	else
		return (false);
	if (!line[i] || is_separator_char_without_quote(line[i]) || (line[i] == '\''
			&& line[i + 1] == 0) || (line[i] == '"' && line[i + 1] == 0))
		return (false);
	else
		return (true);
}

char	**handle_expand(char **tokens, t_data *minishell)
{
	int	t;

	t = 0;
	while (tokens[t])
	{
		tokens[t] = change_expand(tokens[t], minishell, tokens, 0);
		if (!there_is_expand(tokens[t]))
			t++;
	}
	t = 0;
	while (tokens[t])
	{
		tokens[t] = trim_quote(tokens[t]);
		if (!tokens[t])
			malloc_error(minishell, NULL, tokens);
		t++;
	}
	return (tokens);
}
