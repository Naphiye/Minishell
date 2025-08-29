/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 11:18:34 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/18 16:36:45 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/parsing.h"

t_env	*create_new_node(int j, char *envp, int partial)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_substr(envp, 0, j);
	if (!var_name)
	{
		if (partial)
			free(envp);
		return (NULL);
	}
	if (envp[j + 1] == '"')
		var_value = ft_substr(envp, j + 2, (ft_strlen(&envp[j + 1]) - 3));
	else
		var_value = ft_substr(envp, (j + 1), (ft_strlen(envp) - j));
	if (!var_value)
	{
		free(var_name);
		if (partial)
			free(envp);
		return (NULL);
	}
	return (init_node_env(var_name, var_value));
}

char	*gnl_and_init_envp(int fd, t_data *minishell, int *partial_env)
{
	char		*etc_line;
	static int	i = 0;

	*partial_env = 1;
	etc_line = get_next_line(fd);
	if (etc_line == NULL)
		etc_line = create_shlvl(minishell, etc_line);
	if (etc_line)
	{
		if (i == 0)
			minishell->envp = malloc(sizeof(char *) * 2);
		else
			minishell->envp = ft_realloc_tab(minishell->envp);
		if (!minishell->envp)
			get_next_line_and_init_envp_error(fd, minishell, etc_line);
		minishell->envp[i] = malloc(sizeof(char) * (ft_strlen(etc_line) + 1));
		if (!minishell->envp[i])
			get_next_line_and_init_envp_error(fd, minishell, etc_line);
		minishell->envp[i + 1] = NULL;
		ft_strlcpy(minishell->envp[i], etc_line, ft_strlen(etc_line) + 1);
		i++;
	}
	return (etc_line);
}

/* char *gnl_and_init_envp(int fd, t_data *minishell, int *partial_env)
This function handles the case of "getting a simulated environment"
created fromthe /etc/environment file, but not only.
We want to store every line read by gnlfrom this file
in our simulated envp. So we allocate minishell->envpfor the first
line of /etc/environment and a NULL. Then we will reallocate
it one by one eachtime we read a new line from /etc/environment.
Once we've reached the last line,we will add the line "SHLVL=1"
to mimic the aspect of env in an env -i environment.

t_env	*create_new_node(int j, char *envp, int partial)
This function aims to create the t_env structure for our simulated environment.
Keep in mind that even if we are in an "env -i" ora complete env situation,
we want to store the environment data
(which we will gather from the main envp argument in one case and mostly from
/etc/environment in the other case) in two different areas.
The first one:
our homemade envp, a char *envp[] ---> to be used in the execve function.
The second one:
Our structure t_env. The environment data are easier to access in
this structure than in a char *envp[] for the rest of the program.

To sum up: The environment data are gathered from two different places
depending on the situation and are stored in two different places,
also in two different forms (t_env structure and char *envp[]). */