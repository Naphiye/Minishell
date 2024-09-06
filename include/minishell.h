/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 14:21:29 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 15:57:13 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 199309L
# endif
# ifndef _DEFAULT_SOURCE
#  define _DEFAULT_SOURCE
# endif

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SIZE_MAX 200
# define SRC_REDIR 1
# define DST_REDIR 2
# define APPEND 4
# define LAST_FILE 7
# define HERE_DOC 8
# define IN_HEREDOC_PROMPT 9
# define HEREDOC_STOP 10
# define BUILT_IN 16
# define CMD 32
# define FIRST_CMD 64
# define BETWEEN_CMD 128
# define LAST_CMD 256
# define ONLY_ONE_CMD 512
# define NO_CMD 1024
# define NO_SPC 1025
# define CMD_NOT_FOUND 2048
# define NO_SUCH_FILE 4096
# define PERMISSION_DENIED 8192
# define CMD_NOT_EXECUTABLE 16384
# define IS_A_DIRECTORY 16385
# define IS_A_DIRECTORY_CMD 16386
# define DUP2_FAIL 32768
# define OPEN_FAIL 65536
# define PIPE_FAIL 131072
# define FORK_FAIL 262144
# define MALLOC_ERROR 524288
# define HOME_NOT_SET 1048576
# define CHDIR_FAIL 2097152
# define TOO_MANY_ARGUMENTS 4194304
# define DUP_FAIL 8388608
# define NOT_IN_CHILDREN 8388609
# define IN_CHILDREN 8388610
# define CTRL_C 8388611
# define CTRL_BACKSLASH 8388612

typedef struct s_redir_file
{
	char				*file_name;
	int					redir_type;
	int					pos;
	int					pipe[2];
	struct s_redir_file	*next;
}						t_redir_file;

typedef struct s_cmd
{
	char				*cmd_name;
	char				*cmd_path;
	char				**cmd_args;
	int					cmd_type;
	int					cmd_pos;
	t_redir_file		*file;
	int					infilefd;
	int					outfilefd;
	int					last_pipe_read_end;
	int					pipe[2];
	struct s_cmd		*next;
	struct s_cmd		*prev;
	int					save_stdin;
	int					save_stdout;
}						t_cmd;

typedef struct s_env
{
	char				*var_name;
	char				*var_value;
	struct s_env		*prev;
	struct s_env		*next;
}						t_env;

typedef struct s_data
{
	int					in_children;
	int					exit_status;
	t_cmd				*cmd_list;
	t_env				*env;
	char				**envp;
	int					error;
	int					partial_env;
	int					inception_from_partial;
	int					save_stdin_heredoc;
}						t_data;

typedef struct s_into_quote
{
	int					double_quote;
	int					simple_quote;
}						t_into_quote;

typedef struct s_directories_info
{
	char				*parent;
	char				*path;
	char				*file_path;
	int					nbr_folder;
	int					i;
}						t_directories_info;
// signaux

void					listen_signal(t_data *minishell);

// ENV
t_env					*get_env(char *envp[], t_data *minishell);
void					lst_env_clear(t_env *lst_env);
void					ft_lstenvadd_back(t_env **env_lst, t_env *new_node);
t_env					*create_new_node(int j, char *envp, int partial);
void					print_env_lst(t_env *node);
char					*gnl_and_init_envp(int fd, t_data *minishell,
							int *partial_env);
t_env					*init_node_env(void *var_name, void *var_value);
char					*create_shlvl(t_data *minishell, char *etc_line);
t_env					*create_env_with_etc_file(t_env *new, t_data *minishell,
							int fd, t_env *lst_env);
// lst_redir_file
void					lst_redir_file_addback(t_redir_file **file_lst,
							t_redir_file *new_node);
void					lst_redir_file_clear(t_redir_file *file_lst);
t_redir_file			*init_node_lst_file(int redirtype, char *filename);
void					print_redir_file_lst(t_redir_file *node);

// DATA

// INIT
t_data					*ft_init_data(char *envp[], t_data *minishell);
void					init_envp(t_data *minishell, char *new_line);
void					init_old_pwd_envp(t_data *minishell, char *old_pwd);

// MALLOC ERROR
void					error_detected(int *error);
void					clean_all(t_data *minishell);

void					malloc_error(t_data *minishell, char *line,
							char **tokens);
void					malloc_error_env(t_env *lst_env, int fd,
							t_data *minishell);
void					malloc_error_heredoc(t_redir_file *here_doc, int fd,
							char *line_to_free, t_data *minishell);
void					get_next_line_and_init_envp_error(int fd,
							t_data *minishell, char *etc_line);

// LST CMD
t_cmd					*lst_cmd_new_node(void);
void					lst_cmd_clear(t_cmd *cmd_list);
void					lst_cmd_addback(t_cmd **cmd_list, t_cmd *new_node);
t_cmd					*lst_cmd_last(t_cmd *cmd_list);
void					signal_handler(int signum);
// EXEC
void					init_data_for_test_antoine(t_data *minishell);
int						execution(t_data *minishell);
void					child_process(t_cmd *cmd, t_data *minishell);
void					exec_cmd(t_cmd *cmd, t_data *minishell,
							int *exit_status);
int						exec_built_in(t_cmd *cmd, t_data *minishell);

int						wait_for_children_to_end(int exit_status, int pid);
void					close_fd(t_cmd *cmd);
void					find_cmd_path_with_path(t_cmd *cmd, t_data *minishell);
int						check_absolut_or_relative_path(t_cmd *cmd,
							t_data *minishell, char *path);
void					create_absolute_relative_cmd_path(t_cmd *cmd,
							t_data *minishell);

char					*create_path(char *s1, char *s2, char *s3);
void					clean_parent_fd_and_set_last_pipe_read_end(t_cmd *cmd);
char					*get_path_from_env(t_data *minishell);

// PIPE_REDIR
void					last_cmd_case_pipe_redirection(t_cmd *cmd,
							t_data *minishell);
void					between_cmd_case_pipe_redirection(t_cmd *cmd,
							t_data *minishell);
void					first_cmd_case_pipe_redirection(t_cmd *cmd,
							t_data *minishell);
void					pipe_redirection_O_O_CMD_case(t_cmd *cmd,
							t_data *minishell);
void					pipe_redirection(t_cmd *cmd, t_data *minishell);
void					only_one_cmd_case_pipe_redirection(t_cmd *cmd);

// OPERAND_REDIR
void					plug_back_stdin_stdout(t_data *minishell);
int						is_directory(const char *path);

bool					is_only_one_built_in(t_data *minishell);
int						check_dir(char *file_path, t_cmd *cmd,
							t_data *minishell, int is_file_out);

void					operand_redirection(t_cmd *cmd, t_data *minishell);
void					check_operand_file(t_cmd *cmd, t_data *minishell);
void					check_file_in(t_cmd *cmd, t_data *minishell,
							t_redir_file *tmp_file_in);
void					check_file_out(t_cmd *cmd, t_data *minishell,
							t_redir_file *tmp_file_out);
void					file_in_unexist_or_no_acces(t_redir_file *tmp_file_in,
							t_cmd *cmd, t_data *minishell, int err_code);
void					file_in_existing(t_redir_file *tmp_file_in, t_cmd *cmd,
							t_data *minishell);
void					file_out_existing(t_redir_file *tmp_file_out,
							t_cmd *cmd, t_data *minishell);

// CHECK_FILE
void					file_out_unexist_or_no_acces(t_redir_file *tmp_file_out,
							t_cmd *cmd, t_data *minishell, int err_code);
void					file_out_exist(t_redir_file *tmp_file_out, t_cmd *cmd,
							t_data *minishell);

// ERR_MSG
int						print_err_msg_function(int error);
int						error(int err_type, char *msg, t_cmd *cmd,
							t_data *minishell);
int						error_chdir(int err_code, char *path, t_cmd *cmd);
int						error_fct(int err_type, char *elmt, t_cmd *cmd,
							t_data *minishell);
int						print_err_msg(int error);

// PARSING AND EXPORT UTILS
bool					is_this_empty_line(char *input);
char					*trim_quote(char *line);

#endif