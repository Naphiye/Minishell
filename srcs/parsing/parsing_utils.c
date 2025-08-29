/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:36:39 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 11:48:02 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

bool	is_separator_char_without_quote(char c)
{
	if (c == '<' || c == '>' || ft_isspace(c) || c == '|')
		return (true);
	else
		return (false);
}

bool	is_separator_char(char c)
{
	if (c == '<' || c == '>' || ft_isspace(c) || c == '|' || c == '"'
		|| c == 39)
		return (true);
	else
		return (false);
}

// l'idee dans realloc est de rejouter a chaque passage 2 ligne au tableau,
// pour la nouvelle ligne de donne et la deuxieme pour replacer un NULL ?

char	**ft_realloc_tab(char **cmd_args)
{
	char	**tmp;
	int		a;

	a = 1;
	tmp = cmd_args;
	while (cmd_args[a] != NULL)
		a += 1;
	cmd_args = malloc(sizeof(char *) * (a + 2));
	if (!cmd_args)
	{
		ft_free_split(tmp);
		return (NULL);
	}
	a = 0;
	while (tmp[a] != NULL)
	{
		cmd_args[a] = tmp[a];
		a++;
	}
	cmd_args[a] = NULL;
	free(tmp);
	return (cmd_args);
}

static char	*copy_line_without_quote(char *line, char *new_line, int i, int j)
{
	t_into_quote	into;

	into.simple_quote = 0;
	into.double_quote = 0;
	while (line[j])
	{
		change_quote_flag(&into, line[j]);
		if (((line[j] == '\'' && !into.double_quote) || (line[j] == '"'
					&& !into.simple_quote)))
			j++;
		if (!line[j])
			break ;
		if (((line[j] == '\'' && into.double_quote) || (line[j] == '"'
					&& into.simple_quote) || (line[j] != '\''
					&& line[j] != '"')))
		{
			new_line[i] = line[j];
			j++;
			i++;
		}
	}
	free(line);
	new_line[i] = 0;
	return (new_line);
}

char	*trim_quote(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!ft_strchr(line, '"') && !ft_strchr(line, '\''))
		return (line);
	new_line = malloc(sizeof(char) * (ft_strlen(line)));
	if (!new_line)
		return (NULL);
	new_line = copy_line_without_quote(line, new_line, i, j);
	return (new_line);
}
