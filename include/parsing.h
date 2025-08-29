/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:48:58 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 16:39:36 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include <sys/time.h>
# include <time.h>

# define HERE_STRING_ERROR 16384
# define SYNTAX_ERROR 32768
# define STOP_HERE_DOC 65536
# define UNCLOSED_QUOTES 131072

typedef struct s_index
{
	int	index;
	int	new_index;
}		t_index;

// here_doc
char	*process_heredocs(char *input, t_data *minishell);
char	*get_delimiter(char *input, int num_handle);
char	*put_new_value_in_line(char *input, char *new_value, char *old_value,
			int here_doc);
int		create_heredoc_file(t_redir_file *here_doc, t_data *minishell);
bool	is_it_delimiter(char *line, char *del);
bool	there_is_expand(char *line);

// error parsing
void	error_parsing(int flag, char *token, t_data *minishell);
void	stop_here_doc(char *delimiter, int i, t_data *minishell, int *error);

// expand
char	**handle_expand(char **tokens, t_data *minishell);

// expand utils
bool	there_is_expand(char *line);
char	*change_expand(char *line, t_data *minishell, char **tokens,
			int here_doc);

// init_cmd_list
void	init_cmd_list(char **tokens, t_data *minishell);
void	handle_commande(char **tokens, int *t, t_cmd *commande,
			t_data *minishell);
void	handle_pipe(t_cmd *commande, t_data *minishell, int *t);
void	handle_files(char **tokens, int *t, t_cmd *commande, t_data *minishell);
void	init_args(char **tokens, int t, t_cmd *commande, t_data *minishell);
bool	is_redir_token(char *token);
void	set_last_flag(int *file_pos);
void	init_pos_file(t_cmd *cmd);

// input
void	launch_minishell(t_data *minishell);

// parsing utils
bool	is_separator_char_without_quote(char c);
bool	is_separator_char(char c);
char	**ft_realloc_tab(char **cmd_args);

// parsing
void	do_parsing(t_data *minishell, char **tokens, char *input);

// quote
void	change_quote_flag(t_into_quote *into, char c);
bool	not_in_quote(t_into_quote *into);
bool	is_there_unclosed_quote(char *input);

// redir
bool	is_redir(char *line, int *index, t_data *minishell);

// tokens
char	**tokenise_input(char *input, t_data *minishell);

#endif
