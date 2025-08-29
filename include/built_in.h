/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:57:35 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/27 12:56:14 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"
# include <stdbool.h>

# define INVALID_IDENTIFIER 1

// BUILT IN

int		ft_exit(char **args, t_data *data);
int		ft_cd(t_cmd *cmd, t_data *minishell);
int		ft_cd_without_args(t_cmd *cmd, t_data *minishell);
int		ft_env(t_data *minishell);
int		ft_export(char **args, t_data *minishell);
int		ft_pwd(t_cmd *cmd);
int		ft_unset(char **args, t_data *minishell);
int		ft_echo(char **cmd_args, t_data *minishell);

// EXIT
bool	ft_atoll(const char *arg, long long *arg_exit);
int		it_is_only_digit(char *arg);
int		there_is_an_overflow(char *arg, long long *arg_exit);
void	ft_print_exit_error(int error_num, char *args);

// EXPORT
void	export_value(t_data *minishell, char *line_to_export);
void	change_the_value_in_envp(t_data *minishell, char *new_line, int i);
void	add_new_value_to_envp(t_data *minishell, char *new_line);

// ENV
bool	is_inception_from_env_i(t_data *minishell);
#endif