/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:22:39 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/25 16:22:09 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/parsing.h"

extern volatile sig_atomic_t	g_signal_flag;

void	init_old_pwd_envp(t_data *minishell, char *old_pwd)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = getcwd(NULL, 0);
	while (minishell->envp[i])
	{
		if (ft_strncmp("OLDPWD=", minishell->envp[i], 7) == 0)
		{
			minishell->envp[i] = put_new_value_in_line(minishell->envp[i],
					old_pwd, &minishell->envp[i][7], 0);
		}
		else if (ft_strncmp("PWD=", minishell->envp[i], 4) == 0)
		{
			minishell->envp[i] = put_new_value_in_line(minishell->envp[i], tmp,
					&minishell->envp[i][4], 0);
		}
		i++;
	}
	free(tmp);
	free(old_pwd);
}

char	*increment_shlvl(t_data *minishell, char *new_line)
{
	char	*ligne_to_copy;
	int		shlvl_value;
	char	*shlvl_ascii_value;

	shlvl_value = 0;
	shlvl_value = ft_atoi(&new_line[6]);
	shlvl_ascii_value = ft_itoa(shlvl_value + 1);
	if (!shlvl_ascii_value)
		malloc_error(minishell, NULL, NULL);
	ligne_to_copy = ft_strjoin("SHLVL=", shlvl_ascii_value);
	if (!ligne_to_copy)
		malloc_error(minishell, shlvl_ascii_value, NULL);
	free(shlvl_ascii_value);
	return (ligne_to_copy);
}

void	init_envp(t_data *minishell, char *new_line)
{
	static int	i = 0;
	char		*ligne_to_copy;

	if (ft_strncmp("SHLVL=", new_line, 6) == 0)
		ligne_to_copy = increment_shlvl(minishell, new_line);
	else
		ligne_to_copy = new_line;
	if (i == 0)
		minishell->envp = malloc(sizeof(char *) * 2);
	else
		minishell->envp = ft_realloc_tab(minishell->envp);
	if (!minishell->envp)
		malloc_error(minishell, NULL, NULL);
	minishell->envp[i] = malloc(sizeof(char) * (ft_strlen(ligne_to_copy) + 1));
	if (!minishell->envp[i])
		malloc_error(minishell, NULL, NULL);
	ft_strlcpy(minishell->envp[i], ligne_to_copy, ft_strlen(ligne_to_copy) + 1);
	minishell->envp[i + 1] = NULL;
	i++;
	if (ft_strncmp("SHLVL=", new_line, 6) == 0)
		free(ligne_to_copy);
}

t_data	*ft_init_data(char *envp[], t_data *minishell)
{
	int	i;

	i = 0;
	minishell = ft_calloc(sizeof(t_data), 1);
	if (!minishell)
		malloc_error(NULL, NULL, NULL);
	minishell->in_children = NOT_IN_CHILDREN;
	g_signal_flag = NOT_IN_CHILDREN;
	minishell->save_stdin_heredoc = -1;
	minishell->save_stdin_heredoc = dup(STDIN_FILENO);
	if (minishell->save_stdin_heredoc == -1)
	{
		exit(error_fct(DUP_FAIL, "save stdin here_doc", NULL, minishell));
	}
	minishell->env = get_env(envp, minishell);
	return (minishell);
}
