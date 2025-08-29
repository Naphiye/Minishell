/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:48:16 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 14:58:25 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include "../../../headers/parsing.h"

extern volatile sig_atomic_t	g_signal_flag;

static void	add_line_to_file(char *line, int fd, t_data *minishell,
		char *delimiter)
{
	line = change_expand(line, minishell, NULL, 1);
	if (!line)
	{
		close(fd);
		malloc_error(minishell, delimiter, NULL);
	}
	ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
}

static void	read_input_heredoc(int fd, char *delimiter, t_data *minishell)
{
	char	*line;
	int		i;

	i = 0;
	g_signal_flag = IN_HEREDOC_PROMPT;
	line = readline("> ");
	if (line == NULL)
		stop_here_doc(delimiter, i, minishell, &(minishell->error));
	while (line && !is_it_delimiter(line, delimiter)
		&& g_signal_flag != HEREDOC_STOP)
	{
		i++;
		add_line_to_file(line, fd, minishell, delimiter);
		line = readline("> ");
		if (line == NULL)
		{
			if (g_signal_flag == HEREDOC_STOP)
				stop_here_doc(delimiter, i, minishell, &(minishell->error));
			break ;
		}
	}
	if (line)
		free(line);
	close(fd);
	return ;
}

static char	*handle_here_doc(char *input, int num_handle, t_data *minishell)
{
	t_redir_file	*here_doc;
	int				fd_heredoc;
	char			*delimiter;

	here_doc = ft_calloc(sizeof(t_redir_file), 1);
	if (!here_doc)
		malloc_error(minishell, input, NULL);
	fd_heredoc = create_heredoc_file(here_doc, minishell);
	if (fd_heredoc == -1)
		malloc_error_heredoc(here_doc, fd_heredoc, input, minishell);
	delimiter = get_delimiter(input, num_handle);
	if (!delimiter)
		malloc_error_heredoc(here_doc, fd_heredoc, input, minishell);
	input = put_new_value_in_line(input, here_doc->file_name, delimiter, 1);
	if (!input)
		malloc_error_heredoc(here_doc, fd_heredoc, delimiter, minishell);
	read_input_heredoc(fd_heredoc, delimiter, minishell);
	free(delimiter);
	if (minishell->error)
		unlink(here_doc->file_name);
	if (g_signal_flag == HEREDOC_STOP)
		minishell->exit_status = 130;
	lst_redir_file_clear(here_doc);
	return (input);
}

static int	how_many_heredoc(char *input)
{
	int	num_heredoc;
	int	i;

	num_heredoc = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '<' && input[i + 1] == '<')
			num_heredoc++;
		i++;
	}
	return (num_heredoc);
}

char	*process_heredocs(char *input, t_data *minishell)
{
	int	num_heredoc;
	int	i;
	int	num_handle;

	i = 0;
	num_handle = 0;
	num_heredoc = how_many_heredoc(input);
	while (i < num_heredoc)
	{
		input = handle_here_doc(input, num_handle, minishell);
		if (g_signal_flag == HEREDOC_STOP)
			break ;
		i++;
		num_handle++;
	}
	g_signal_flag = NOT_IN_CHILDREN;
	if (dup2(minishell->save_stdin_heredoc, STDIN_FILENO) == -1)
	{
		close(minishell->save_stdin_heredoc);
		exit(error_fct(DUP2_FAIL, "replug stdin", NULL, minishell));
	}
	close(minishell->save_stdin_heredoc);
	minishell->save_stdin_heredoc = -1;
	return (input);
}
