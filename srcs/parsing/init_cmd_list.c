/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:05:34 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:46:31 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

static bool	is_command_token(char **token, int t, int e, t_data *minishell)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if ((!is_redir(token[t], &i, minishell) && (e == 0) && token[t][0] != '|')
		|| ((e != 0) && !is_redir(token[t - 1], &j, minishell)
			&& token[t][0] != '|'))
		return (true);
	else
		return (false);
}

bool	is_redir_token(char *token)
{
	if (token[0] == '>' || token[0] == '<')
	{
		if (!token[1])
			return (true);
		else if (token[1] == token[0] && !token[2])
			return (true);
		else
			return (false);
	}
	return (false);
}

static bool	is_pipe_token(char **tokens, int t, t_data *minishell, int *error)
{
	if (tokens[t][0] == '|' && tokens[t][1] == 0)
	{
		if (t == 0 || tokens[t + 1] == NULL)
		{
			error_parsing(SYNTAX_ERROR, tokens[t], minishell);
			*error = 1;
			return (false);
		}
		else
			return (true);
	}
	return (false);
}

static void	recognize_and_handle_tokens(char **tokens, t_data *minishell,
		int *t, t_cmd *commande)
{
	int	e;

	e = 0;
	while (is_this_empty_line(tokens[*t]) && tokens[*t + 1] != NULL)
		*t += 1;
	while (!minishell->error && tokens && tokens[*t])
	{
		if (commande->cmd_name == NULL && is_command_token(tokens, *t, e,
				minishell))
			handle_commande(tokens, t, commande, minishell);
		else if (is_redir_token(tokens[*t]))
			handle_files(tokens, t, commande, minishell);
		else if (is_pipe_token(tokens, *t, minishell, &minishell->error))
		{
			handle_pipe(commande, minishell, t);
			break ;
		}
		else if (!minishell->error)
		{
			init_args(tokens, *t, commande, minishell);
			*t += 1;
		}
		e++;
	}
}

void	init_cmd_list(char **tokens, t_data *minishell)
{
	t_cmd	*commande;
	int		t;

	t = 0;
	commande = NULL;
	while (1)
	{
		if (minishell->error)
			break ;
		if (tokens && tokens[t])
		{
			commande = lst_cmd_new_node();
			if (!commande)
				malloc_error(minishell, NULL, tokens);
		}
		else
		{
			if (commande)
				lst_cmd_addback(&minishell->cmd_list, commande);
			break ;
		}
		recognize_and_handle_tokens(tokens, minishell, &t, commande);
	}
	if (minishell->error && commande)
		lst_cmd_addback(&minishell->cmd_list, commande);
}
