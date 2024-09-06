/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 09:47:55 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/25 09:55:32 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"

// CREER UNE FONCTION QUI REMPLI LA STRUCUTRE
// POUR POUVOIR UTILISER LA STRUCTURE EN ARGUMENT DES BUILTIN
// IL FAUDRA EGALEMENT GERER LES ERREURS EN CAS D"ECHEC D"EXECUTION //

static int	is_a_noption(char *args)
{
	int	i;

	i = 0;
	if (args[i] == '-')
	{
		i++;
		if (args[i++] == 'n')
		{
			if (args[i] && args[i] == 'n')
			{
				while (args[i] && args[i] == 'n')
					i++;
				if (args[i] == '\0')
					return (true);
				else
					return (false);
			}
			else if (args[i] && args[i] != 'n')
				return (false);
			else
				return (true);
		}
		return (false);
	}
	return (false);
}

static void	noption(char **cmd_args, int *i, int *nflag)
{
	*nflag = false;
	while (cmd_args[*i] && is_a_noption(cmd_args[*i]))
	{
		*i += 1;
		*nflag = true;
	}
}

static void	strlen_output(char **cmd_args, int *len_output, int i)
{
	*len_output = 0;
	while (cmd_args[i])
	{
		*len_output += ft_strlen(cmd_args[i]);
		if (cmd_args[i + 1] != NULL)
			*len_output += 1;
		i++;
	}
}

void	create_output(char **cmd_args, char **output, int i)
{
	while (cmd_args[i])
	{
		ft_strcat(*output, cmd_args[i]);
		if (cmd_args[i + 1] != NULL)
			ft_strcat(*output, " ");
		i++;
	}
}

int	ft_echo(char **cmd_args, t_data *minishell)
{
	int		i;
	int		len_output;
	char	*output;
	int		nflag;

	nflag = 0;
	len_output = 0;
	i = 1;
	if (cmd_args[i] == NULL)
		return (ft_putstr_fd("\n", STDOUT_FILENO), 0);
	noption(cmd_args, &i, &nflag);
	if (cmd_args[i] == NULL)
		return (0);
	strlen_output(cmd_args, &len_output, i);
	output = ft_calloc(sizeof(char), len_output + 1);
	if (output == NULL)
		malloc_error(minishell, NULL, NULL);
	create_output(cmd_args, &output, i);
	ft_putstr_fd(output, STDOUT_FILENO);
	if (nflag == 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	free(output);
	return (0);
}
