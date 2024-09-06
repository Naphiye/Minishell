/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_new_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 12:00:15 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/23 17:03:43 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include "../../../headers/parsing.h"

static char	*allocate_new_input(char *input, char *new_value, char *old_value)
{
	int		new_input_len;
	char	*new_input;

	if (!new_value)
	{
		new_input_len = (ft_strlen(input) - ft_strlen(old_value));
	}
	else
		new_input_len = ft_strlen(input) + ((ft_strlen(new_value)
					- (ft_strlen(old_value))));
	new_input = ft_calloc(sizeof(char), (new_input_len + 1));
	if (!new_input)
		return (NULL);
	return (new_input);
}

static int	copy_first_part_input(char *new_input, char *input, int i,
		char *del)
{
	int		input_len;
	char	*del_start;

	input_len = ft_strlen(input);
	del_start = ft_strnstr(input, del, input_len);
	while (&input[i] != del_start)
	{
		new_input[i] = input[i];
		i++;
	}
	return (i);
}

static int	copy_new_value_input(char *new_input, char *new_value, int i)
{
	int	d;

	d = 0;
	if (new_value)
	{
		while (new_value[d] != 0)
		{
			new_input[i + d] = new_value[d];
			d++;
		}
	}
	return (i + d);
}

char	*put_new_value_in_line(char *input, char *new_value, char *old_value,
		int here_doc)
{
	int		i;
	int		j;
	char	*new_input;

	i = 0;
	new_input = allocate_new_input(input, new_value, old_value);
	if (!new_input)
	{
		if (here_doc)
			free(input);
		return (NULL);
	}
	i = copy_first_part_input(new_input, input, i, old_value);
	j = copy_new_value_input(new_input, new_value, i);
	i += ft_strlen(old_value);
	while (input[i] != 0)
	{
		new_input[j] = input[i];
		j++;
		i++;
	}
	free(input);
	return (new_input);
}
