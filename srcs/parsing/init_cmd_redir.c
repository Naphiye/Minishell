/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 17:36:55 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 15:09:06 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

static void	init_outfile_redir(char **tokens, t_cmd *commande, int t,
		t_data *minishell)
{
	t_redir_file	*new_file;
	int				redir_type;
	char			*file_name;

	file_name = ft_substr(tokens[t + 1], 0, ft_strlen(tokens[t + 1]));
	if (!file_name)
	{
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	if (ft_strlen(tokens[t]) == 2)
		redir_type = APPEND;
	else
		redir_type = DST_REDIR;
	new_file = init_node_lst_file(redir_type, file_name);
	if (!new_file)
	{
		free(file_name);
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	lst_redir_file_addback(&commande->file, new_file);
}

static void	init_infile_redir(char **tokens, t_cmd *commande, int t,
		t_data *minishell)
{
	t_redir_file	*new_file;
	int				redir_type;
	char			*file_name;

	file_name = ft_substr(tokens[t + 1], 0, ft_strlen(tokens[t + 1]));
	if (!file_name)
	{
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	if (ft_strlen(tokens[t]) == 2)
		redir_type = HERE_DOC;
	else
		redir_type = SRC_REDIR;
	new_file = init_node_lst_file(redir_type, file_name);
	if (!new_file)
	{
		free(file_name);
		lst_cmd_clear(commande);
		malloc_error(minishell, NULL, tokens);
	}
	lst_redir_file_addback(&commande->file, new_file);
}

static bool	is_outfile_redir(char *token)
{
	if (token[0] == '>')
		return (true);
	else
		return (false);
}

static bool	is_infile_redir(char *token)
{
	if (token[0] == '<')
		return (true);
	else
		return (false);
}

void	handle_files(char **tokens, int *t, t_cmd *commande, t_data *minishell)
{
	if (!tokens[*t + 1] || is_redir_token(tokens[*t + 1]))
	{
		error_detected(&minishell->error);
		if (tokens[*t + 1])
			error_parsing(SYNTAX_ERROR, tokens[*t + 1], minishell);
		else
			error_parsing(SYNTAX_ERROR, NULL, minishell);
	}
	else if (is_infile_redir(tokens[*t]))
	{
		init_infile_redir(tokens, commande, *t, minishell);
		*t += 2;
	}
	else if (is_outfile_redir(tokens[*t]))
	{
		init_outfile_redir(tokens, commande, *t, minishell);
		*t += 2;
	}
	init_pos_file(commande);
}
