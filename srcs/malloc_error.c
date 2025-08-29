/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 11:19:03 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 15:36:52 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

void	error_detected(int *error)
{
	*error = 1;
	return ;
}

void	clean_all(t_data *minishell)
{
	if (minishell->cmd_list)
		lst_cmd_clear(minishell->cmd_list);
	if (minishell->env)
		lst_env_clear(minishell->env);
	if (minishell->envp)
		ft_free_split(minishell->envp);
	if (minishell->save_stdin_heredoc != -1)
		close(minishell->save_stdin_heredoc);
	free(minishell);
}

void	malloc_error(t_data *minishell, char *line, char **tokens)
{
	if (line)
		free(line);
	if (tokens)
		ft_free_split(tokens);
	if (minishell)
		clean_all(minishell);
	ft_putstr_fd("malloc error\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	malloc_error_env(t_env *lst_env, int fd, t_data *minishell)
{
	if (fd != -1)
		close(fd);
	lst_env_clear(lst_env);
	malloc_error(minishell, NULL, NULL);
}

void	malloc_error_heredoc(t_redir_file *here_doc, int fd, char *line_to_free,
		t_data *minishell)
{
	if (here_doc)
		lst_redir_file_clear(here_doc);
	if (fd != -1)
		close(fd);
	malloc_error(minishell, line_to_free, NULL);
}
