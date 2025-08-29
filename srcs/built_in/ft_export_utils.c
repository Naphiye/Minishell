/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:26:22 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/23 18:33:39 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

void	change_the_value_in_envp(t_data *minishell, char *new_line, int i)
{
	if (ft_strlen(new_line) > SIZE_MAX)
		error_fct(NO_SPC, "export", minishell->cmd_list, minishell);
	free(minishell->envp[i]);
	minishell->envp[i] = ft_strdup(new_line);
	if (!minishell->envp[i])
		malloc_error(minishell, NULL, NULL);
}

void	add_new_value_to_envp(t_data *minishell, char *new_line)
{
	int	i;

	i = 0;
	while (minishell->envp[i])
		i++;
	if (i > SIZE_MAX || ft_strlen(new_line) > SIZE_MAX)
		error_fct(NO_SPC, "export", minishell->cmd_list, minishell);
	minishell->envp = ft_realloc_tab(minishell->envp);
	if (!minishell->envp)
		malloc_error(minishell, NULL, NULL);
	minishell->envp[i] = malloc(sizeof(char) * (ft_strlen(new_line) + 1));
	if (!minishell->envp[i])
		malloc_error(minishell, NULL, NULL);
	ft_strlcpy(minishell->envp[i], new_line, ft_strlen(new_line) + 1);
	minishell->envp[i + 1] = NULL;
}
