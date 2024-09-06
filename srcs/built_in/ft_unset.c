/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 16:46:51 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/24 13:41:09 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

static void	unset_struct_env(t_data *minishell, char *unset)
{
	t_env	*tmp;

	tmp = minishell->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->var_name, unset) == 0)
		{
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				minishell->env = tmp->next;
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			tmp->prev = NULL;
			tmp->next = NULL;
			lst_env_clear(tmp);
			break ;
		}
		tmp = tmp->next;
	}
}

static void	unset_envp(t_data *minishell, char *unset)
{
	int	i;
	int	len_to_compare;

	i = 0;
	len_to_compare = ft_strlen(unset);
	while (minishell->envp && minishell->envp[i])
	{
		if (ft_strncmp(minishell->envp[i], unset, len_to_compare) == 0)
			break ;
		i++;
	}
	unset_struct_env(minishell, unset);
	free(minishell->envp[i]);
	while (minishell->envp[i + 1])
	{
		minishell->envp[i] = minishell->envp[i + 1];
		i++;
	}
	minishell->envp[i] = NULL;
}

static bool	is_there_a_value_to_unset(char **envp, char *unset)
{
	int	i;
	int	len_to_compare;

	i = 0;
	len_to_compare = ft_strlen(unset);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], unset, len_to_compare) == 0)
			return (true);
		i++;
	}
	return (false);
}

int	ft_unset(char **args, t_data *minishell)
{
	int	i;

	i = 1;
	while (args && args[i] != NULL)
	{
		if (is_there_a_value_to_unset(minishell->envp, args[i]))
			unset_envp(minishell, args[i]);
		i++;
	}
	return (0);
}
