/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 17:13:48 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:06:28 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

static char	*extract_value(char *expand, char **env)
{
	char	*value;
	int		value_len;
	int		line;
	int		i;
	char	*res;

	line = 0;
	i = 0;
	value = NULL;
	while (env && env[line])
	{
		res = ft_strnstr(env[line], &expand[1], ft_strlen(env[line]));
		if (res && res == &env[line][i] && res[ft_strlen(&expand[1])] == '=')
		{
			value_len = ft_strlen(env[line]) - (ft_strlen(&expand[1]) + 1);
			value = malloc(sizeof(char) + (value_len + 1));
			if (!value)
				return (NULL);
			res += (ft_strlen(&expand[1]) + 1);
			ft_strlcpy(value, res, value_len + 1);
			return (value);
		}
		line++;
	}
	return (NULL);
}

static char	*get_value(char *expand, t_data *minishell)
{
	char	*value;

	value = extract_value(expand, minishell->envp);
	if (value)
		return (value);
	return (NULL);
}

static int	get_expand_len(char *line, int i)
{
	int	len_expand;

	len_expand = 2;
	while (line[i] && !is_separator_char(line[i]) && line[i] != '='
		&& line[i] != '$')
	{
		len_expand++;
		if (line[i] == '?')
			break ;
		i++;
	}
	return (len_expand);
}

static char	*get_expand(char *line, t_data *minishell, char **tokens)
{
	char			*expand;
	int				i;
	int				len_expand;
	int				index;
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
	index = i;
	i++;
	len_expand = get_expand_len(line, i);
	expand = malloc(sizeof(char) * (len_expand + 1));
	if (!expand)
		malloc_error(minishell, line, tokens);
	ft_strlcpy(expand, &line[index], len_expand);
	return (expand);
}

char	*change_expand(char *token, t_data *minishell, char **tokens,
		int here_doc)
{
	char	*expand;
	char	*new_value;
	char	*newtoken;

	if (there_is_expand(token))
		expand = get_expand(token, minishell, tokens);
	else
		return (token);
	if (expand[1] == '?')
		new_value = ft_itoa(minishell->exit_status);
	else
		new_value = get_value(expand, minishell);
	newtoken = put_new_value_in_line(token, new_value, expand, 0);
	free(expand);
	if (new_value)
		free(new_value);
	if (!newtoken && !here_doc)
		malloc_error(minishell, NULL, tokens);
	return (newtoken);
}
