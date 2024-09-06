/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 17:15:22 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:45:22 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

static bool	is_normal_char(char *line, int *index)
{
	t_into_quote	into;

	into.simple_quote = 0;
	into.double_quote = 0;
	if (line[*index] && !is_separator_char_without_quote(line[*index]))
	{
		while (line[*index] && (!is_separator_char_without_quote(line[*index])
				|| into.double_quote || into.simple_quote))
		{
			change_quote_flag(&into, line[*index]);
			*index += 1;
		}
		return (true);
	}
	else
		return (false);
}

static bool	is_pipe(char *line, int *index, t_data *minishell, int *error)
{
	char	c;

	c = line[*index];
	if (c == '|')
	{
		if (line[*index + 1] == c)
		{
			error_parsing(SYNTAX_ERROR, &c, minishell);
			*error = 1;
			return (true);
		}
		else
		{
			*index += 1;
			return (true);
		}
	}
	else
		return (false);
}

static char	*allocate_and_copy_token(t_index *index, char *line, char **tokens,
		t_data *minishell)
{
	int		token_size;
	char	*token;

	token_size = index->new_index - index->index;
	token = malloc(sizeof(char) * (token_size + 1));
	if (!token)
		malloc_error(minishell, line, tokens);
	ft_strlcpy(token, &line[index->index], token_size + 1);
	return (token);
}

static char	*split_input(char *line, int *i, t_data *minishell, char **tokens)
{
	char	*token;
	t_index	index;

	token = NULL;
	index.new_index = *i;
	index.index = *i;
	if (is_redir(line, &(index.new_index), minishell))
		token = allocate_and_copy_token(&index, line, tokens, minishell);
	else if (is_pipe(line, &(index.new_index), minishell, &minishell->error)
		&& minishell->error != 1)
		token = allocate_and_copy_token(&index, line, tokens, minishell);
	else if (is_normal_char(line, &(index.new_index)) && !minishell->error)
		token = allocate_and_copy_token(&index, line, tokens, minishell);
	*i = index.new_index;
	if (token)
		return (token);
	else
		return (NULL);
}

char	**tokenise_input(char *input, t_data *minishell)
{
	char	**tokens;
	int		t;
	int		i;
	int		input_len;

	t = 0;
	i = 0;
	input_len = ft_strlen(input);
	tokens = malloc(sizeof(char *) * (input_len + 1));
	if (!tokens)
		malloc_error(minishell, input, NULL);
	while (input[i] && i < input_len && !minishell->error)
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i])
		{
			tokens[t] = split_input(input, &i, minishell, tokens);
			if (!tokens[t])
				malloc_error(minishell, input, tokens);
			t++;
		}
	}
	tokens[t] = NULL;
	return (tokens);
}
