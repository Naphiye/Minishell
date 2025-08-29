/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anastruc <anastruc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 12:09:52 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 11:36:58 by anastruc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

bool	is_inception_from_env_i(t_data *minishell)
{
	int	i;

	i = 0;
	if (minishell->inception_from_partial == 1)
		return (true);
	while (minishell->envp[i] && ft_strncmp("PATH=", minishell->envp[i],
			5) != 0)
		i++;
	if (minishell->envp[i])
	{
		if (minishell->envp[i][5] == '"')
		{
			minishell->inception_from_partial = 1;
			return (true);
		}
		return (false);
	}
	return (false);
}

int	ft_env(t_data *minishell)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = minishell->env;
	if (minishell->partial_env || is_inception_from_env_i(minishell))
	{
		printf("PWD=");
		if (ft_pwd(minishell->cmd_list) == 1)
			return (1);
		while (minishell->envp[i])
		{
			if (ft_strncmp("PATH=", minishell->envp[i], 5) != 0)
				printf("%s\n", minishell->envp[i]);
			i++;
		}
		printf("_=/usr/bin/env\n");
	}
	else
	{
		while (minishell->envp[i])
			printf("%s\n", minishell->envp[i++]);
	}
	return (0);
}

/* Lors de l'ouverture d'un minishell dans un minishell et de l'execution
 de Env,
	si le premier minishell a ete ouvert en env -i,
	le shlvl 2 execute bien un env avec un
partial env a 1 ce qui donne un resultat coherent. En revanche,
	si je relance un minishel dans un minishell,
	minishell->partial env est reinitialise a 0.
Donc env imprime la valeur de envp[i],
sauf que envpi a ete prealablement set a gnl etc_line de etc/environnement.
En effet on mettait a jour envp avec la valeur de etc pour pouvoir apres
 initialiser les nodes de la meme manieres dans les deux situation.
Je pense qu'il faudrait un moyen de transferer le fait que l'on est dans
un partail env dans l'inception.
 Auequel cas il faudrait modifier nos fonction de free pour garder cette info*/