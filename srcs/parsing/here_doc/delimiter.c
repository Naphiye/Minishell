/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiter.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 11:57:58 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/13 13:02:52 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include "../../../headers/parsing.h"

static bool	is_quote_char(char c, int *flag)
{
	if (c == '"')
	{
		if (flag && *flag)
			*flag = 0;
		else if (flag)
			*flag = 1;
		return (true);
	}
	return (false);
}

static int	get_delimiter_len(char *input_start, int len, int *quote_flag)
{
	int	i;

	i = 0;
	while (input_start[i])
	{
		if (is_quote_char(input_start[i], quote_flag))
		{
			i++;
			while (input_start[i] && !is_quote_char(input_start[i], NULL))
			{
				len++;
				i++;
				*quote_flag = 1;
			}
			break ;
		}
		else if (ft_isspace(input_start[i])
			|| is_separator_char(input_start[i]))
			break ;
		else
			i++;
		len++;
	}
	return (len);
}

static char	*copy_delimiter(char *input_start)
{
	char	*delimiter;
	int		len;
	int		quote_flag;

	len = 1;
	quote_flag = 0;
	len = get_delimiter_len(input_start, len, &quote_flag);
	delimiter = malloc(sizeof(char) * (len + 1));
	if (!delimiter)
		return (NULL);
	ft_strlcpy(delimiter, &input_start[quote_flag], len);
	return (delimiter);
}

char	*get_delimiter(char *input, int num_handle)
{
	int		i;
	int		quote_flag;
	char	*del;
	int		num_find;

	i = 0;
	num_find = 0;
	quote_flag = 0;
	while (input[i])
	{
		if (is_quote_char(input[i], &quote_flag))
			i++;
		else if (input[i] == '<' && input[i + 1] == '<' && !quote_flag)
		{
			i += 2;
			while (ft_isspace(input[i]))
				i++;
			if (num_find == num_handle)
				return (del = copy_delimiter(&input[i]));
			num_find++;
		}
		else
			i++;
	}
	return (NULL);
}

bool	is_it_delimiter(char *line, char *del)
{
	if (ft_strcmp(line, del) == 0)
		return (true);
	else
		return (false);
}
