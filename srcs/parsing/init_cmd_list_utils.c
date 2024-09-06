/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_list_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:18:08 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/13 12:02:32 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

static bool	is_built_in(char *token)
{
	if (ft_strcmp(token, "exit") == 0)
		return (true);
	if (ft_strcmp(token, "env") == 0)
		return (true);
	if (ft_strcmp(token, "echo") == 0)
		return (true);
	if (ft_strcmp(token, "pwd") == 0)
		return (true);
	if (ft_strcmp(token, "export") == 0)
		return (true);
	if (ft_strcmp(token, "unset") == 0)
		return (true);
	if (ft_strcmp(token, "cd") == 0)
		return (true);
	else
		return (false);
}

static int	determine_cmd_type(char *token)
{
	if (is_built_in(token))
		return (BUILT_IN);
	else
		return (CMD);
}

void	handle_commande(char **tokens, int *t, t_cmd *commande,
		t_data *minishell)
{
	int	len;

	len = ft_strlen(tokens[*t]);
	commande->cmd_name = ft_strdup(tokens[*t]);
	commande->cmd_args = ft_calloc(sizeof(char *), 2);
	if (!commande->cmd_args || !commande->cmd_name)
	{
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	commande->cmd_type = determine_cmd_type(tokens[*t]);
	commande->cmd_args[0] = ft_strdup(tokens[*t]);
	if (!commande->cmd_args[0])
	{
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	commande->cmd_args[1] = NULL;
	*t += 1;
}

void	handle_pipe(t_cmd *commande, t_data *minishell, int *t)
{
	lst_cmd_addback(&minishell->cmd_list, commande);
	commande = NULL;
	*t += 1;
	return ;
}

void	init_args(char **tokens, int t, t_cmd *commande, t_data *minishell)
{
	char	*args;
	int		a;
	int		len;

	len = ft_strlen(tokens[t]);
	a = 0;
	commande->cmd_args = ft_realloc_tab(commande->cmd_args);
	args = ft_substr(tokens[t], 0, len);
	if (!commande->cmd_args || !args)
	{
		if (args)
			free(args);
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	while (commande->cmd_args[a] != NULL)
		a += 1;
	commande->cmd_args[a] = args;
	commande->cmd_args[a + 1] = NULL;
}
