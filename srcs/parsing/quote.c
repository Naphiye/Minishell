/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:46:06 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/24 11:51:57 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

bool	is_there_unclosed_quote(char *input)
{
	int				i;
	int				num_quote;
	t_into_quote	into;

	into.simple_quote = 0;
	into.double_quote = 0;
	num_quote = 0;
	i = 0;
	while (input[i])
	{
		change_quote_flag(&into, input[i]);
		if (input[i] == '"' && !into.simple_quote)
			num_quote += 1;
		else if (input[i] == '\'' && !into.double_quote)
			num_quote += 1;
		i++;
	}
	if (num_quote % 2 == 0)
		return (false);
	return (true);
}

void	change_quote_flag(t_into_quote *into, char c)
{
	if (c == '\'' && !into->double_quote)
	{
		if (into->simple_quote)
			into->simple_quote = 0;
		else
			into->simple_quote = 1;
	}
	else if (c == '"' && !into->simple_quote)
	{
		if (into->double_quote)
			into->double_quote = 0;
		else
			into->double_quote = 1;
	}
}

bool	not_in_quote(t_into_quote *into)
{
	if (into->double_quote == 0 && into->simple_quote == 0)
		return (true);
	else
		return (false);
}
