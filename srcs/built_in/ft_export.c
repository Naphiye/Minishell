/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 15:46:02 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 12:55:35 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

static void	error_export(int flag, char *id)
{
	if (flag == INVALID_IDENTIFIER)
	{
		ft_putstr_fd("bash: export: `", STDERR_FILENO);
		ft_putstr_fd(id, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	}
}

static bool	is_there_a_value_to_export(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '=')
			return (true);
		i++;
	}
	return (false);
}

static bool	is_this_valid_identifier(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (i == 0 && !ft_isalpha(line[i]) && line[i] != '_')
		{
			error_export(INVALID_IDENTIFIER, line);
			return (false);
		}
		else if (line[i] == '=')
			break ;
		else if (!ft_isdigit(line[i]) && line[i] != '_' && !ft_isalpha(line[i]))
		{
			error_export(INVALID_IDENTIFIER, line);
			return (false);
		}
		else
			i++;
	}
	return (true);
}

void	export_value(t_data *minishell, char *line_to_export)
{
	int	i;
	int	name_len;
	int	xport_exis;

	xport_exis = 0;
	i = 0;
	name_len = 0;
	while (line_to_export[name_len] != '=')
		name_len++;
	while ((minishell->envp[i] != 0 && !xport_exis))
	{
		if (ft_strncmp(minishell->envp[i], line_to_export, name_len + 1) == 0)
		{
			xport_exis = 1;
			break ;
		}
		i++;
	}
	if (!line_to_export)
		malloc_error(minishell, NULL, NULL);
	if (minishell->envp[i] != 0)
		change_the_value_in_envp(minishell, line_to_export, i);
	else
		add_new_value_to_envp(minishell, line_to_export);
}

int	ft_export(char **args, t_data *minishell)
{
	int	i;
	int	error;

	error = 0;
	i = 1;
	while (args && args[i] != NULL)
	{
		if (is_this_valid_identifier(args[i]))
		{
			if (is_there_a_value_to_export(args[i]))
				export_value(minishell, args[i]);
		}
		else
			error = 1;
		i++;
		if (minishell->error)
			return (minishell->exit_status);
	}
	return (error);
}
