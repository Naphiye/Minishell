/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:45:51 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/27 12:56:24 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/built_in.h"
#include "../../headers/minishell.h"

void	create_pwd_node(t_data *minishell, char *old_pwd, char *old_pwd_name)
{
	t_env	*new;

	old_pwd_name = ft_substr("OLDPWD", 0, 7);
	if (!old_pwd_name)
		malloc_error(minishell, old_pwd, NULL);
	new = init_node_env(old_pwd_name, old_pwd);
	if (!new)
	{
		free(old_pwd);
		malloc_error(minishell, old_pwd_name, NULL);
	}
	ft_lstenvadd_back(&minishell->env, new);
}

void	ft_cd_init_old_actual_pwd(t_data *minishell, t_env *tmp, char *old_pwd)
{
	char	*old_pwd_name;
	char	*old_pwd_line;

	old_pwd_name = NULL;
	if (minishell->partial_env || is_inception_from_env_i(minishell))
	{
		if (tmp)
		{
			free(tmp->var_value);
			tmp->var_value = old_pwd;
		}
		else
			create_pwd_node(minishell, old_pwd, old_pwd_name);
		old_pwd_line = ft_strjoin("OLDPWD=", old_pwd);
		if (!old_pwd_line)
		{
			free(old_pwd);
			malloc_error(minishell, old_pwd_name, NULL);
		}
		export_value(minishell, old_pwd_line);
		free(old_pwd_line);
	}
	else
		init_old_pwd_envp(minishell, old_pwd);
}

int	ft_cd_without_args(t_cmd *cmd, t_data *minishell)
{
	int		err_code;
	t_env	*tmp;

	tmp = minishell->env;
	while (tmp && ft_strcmp(tmp->var_name, "HOME") != 0)
		tmp = tmp->next;
	if (tmp == NULL)
		return (error_chdir(HOME_NOT_SET, NULL, cmd));
	else
	{
		if (chdir(tmp->var_value) != 0)
		{
			err_code = errno;
			return (error_chdir(err_code, tmp->var_value, cmd));
		}
		return (0);
	}
}

int	ft_cd_with_args(t_data *minishell, t_cmd *cmd, char *old_pwd, t_env *tmp)
{
	int	err_code;

	if (access(cmd->cmd_args[1], F_OK | X_OK) == 0)
	{
		old_pwd = getcwd(NULL, 0);
		if (old_pwd == NULL)
		{
			err_code = errno;
			return (error_chdir(err_code, cmd->cmd_args[1], cmd));
		}
		if (chdir(cmd->cmd_args[1]) != 0)
		{
			err_code = errno;
			free(old_pwd);
			return (error_chdir(err_code, cmd->cmd_args[1], cmd));
		}
		ft_cd_init_old_actual_pwd(minishell, tmp, old_pwd);
	}
	else
	{
		err_code = errno;
		return (error_chdir(err_code, cmd->cmd_args[1], cmd));
	}
	return (0);
}

int	ft_cd(t_cmd *cmd, t_data *minishell)
{
	int		err_code;
	char	*old_pwd;
	t_env	*tmp;

	old_pwd = NULL;
	tmp = minishell->env;
	while (tmp && ft_strcmp(tmp->var_name, "OLDPWD") != 0)
		tmp = tmp->next;
	err_code = 0;
	if (cmd->cmd_args[1] != NULL && cmd->cmd_args[2] != NULL)
		return (error_chdir(TOO_MANY_ARGUMENTS, NULL, cmd));
	if (cmd->cmd_args[1] == NULL)
		return (ft_cd_without_args(cmd, minishell));
	else
		return (ft_cd_with_args(minishell, cmd, old_pwd, tmp));
	return (0);
}

/* If cd has no arguments (args[1] == NULL),
the program will search for HOME in the environment and try to access this
directory (normal behavior).
In the case of env -i, where HOME has not been set,
the error code will be "HOME NOT SET".
Otherwise, if "cd" has arguments,
the function chdir will be called with the argument. cd takes only one
argument which is an absolute or relative path.
On success,
0 is returned. As soon as the user has changed the directory
from the current one, two values are set or updated
in the environment: OLDPWD and PWD.
If the environment is partial, as in the case of "env -i",
the simulated environment, the t_env structure is updated.
If there is already an OLDPWD in the structure,
the value is updated using
the getcwd function, whose result has been stored in the variable old_pwd.
Otherwise, the node is created and the value is set in the same way.

init_old_pwd_envp(minishell, old_pwd): (see init directory)
If we are in a complete environment situation (bash launched without env -i),
in this case,
we'll simply update the value of the corresponding lines for PWD= and
OLDPWD= with the correct  values: PWD=getcwd(NULL, 0) and
OLDPWD=old_pwd
(obtained with getcwd before changing directory with chdir());
 */