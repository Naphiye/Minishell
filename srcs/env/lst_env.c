/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:19:29 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/18 16:30:02 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static int	open_etc_env(t_data *minishell)
{
	int	fd;

	fd = open("/etc/environment", O_RDONLY);
	if (fd == -1)
		malloc_error(minishell, NULL, NULL);
	return (fd);
}

static t_env	*get_partial_env(t_env *new, t_data *minishell)
{
	t_env	*lst_env;
	int		fd;

	lst_env = NULL;
	fd = open_etc_env(minishell);
	lst_env = create_env_with_etc_file(new, minishell, fd, lst_env);
	close(fd);
	return (lst_env);
}

t_env	*get_env(char *envp[], t_data *minishell)
{
	t_env	*lst_env;
	int		i;
	int		j;
	t_env	*new;

	i = 0;
	j = 0;
	new = NULL;
	lst_env = NULL;
	if (envp[0] == NULL)
		return (get_partial_env(new, minishell));
	while (envp[i])
	{
		while (envp[i][j] != '\0' && envp[i][j] != '=')
			j++;
		new = create_new_node(j, envp[i], 0);
		init_envp(minishell, envp[i]);
		if (new == NULL)
			malloc_error_env(lst_env, -1, minishell);
		ft_lstenvadd_back(&lst_env, new);
		j = 0;
		i++;
	}
	return (lst_env);
}

void	lst_env_clear(t_env *lst_env)
{
	t_env	*tmp;

	while (lst_env)
	{
		tmp = lst_env;
		lst_env = lst_env->next;
		if (tmp->var_name)
			free(tmp->var_name);
		if (tmp->var_value)
			free(tmp->var_value);
		free(tmp);
	}
}

/* We get the env from two different spaces.
If envp can be access, (envp[0] does exit)
we init the t_env chained list from envp (main argument).
Otherwise we init t_env with datas from etc/environment.
The main purposed of this structured is to be be used in the exec part to check
the accessibility of the command with
the PATH=  from the env t_env before executing the function "execve".
However, if the shell is executed with "env -i",
we need to init a simulated envp var (char *envp[]) to replace the actual envp.
Why ?
In order to execute execve with a (char *envp[])
different from envp[0]=NULL (env-i situation).
Now if we execute a minishell in our minishell,
the second level will inherit of a simili envp.*/
