/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_name.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:58:05 by fsalomon          #+#    #+#             */
/*   Updated: 2024/07/26 16:43:52 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include "../../../headers/parsing.h"

static int	get_random_number(t_data *minishell)
{
	int	fd;
	int	random_value;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		clean_all(minishell);
		exit(EXIT_FAILURE);
	}
	if (read(fd, &random_value, sizeof(int)) != sizeof(int))
	{
		perror("read");
		close(fd);
		clean_all(minishell);
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (random_value);
}

static char	*get_random_name(t_data *minishell)
{
	char	*file_name;
	int		random_num;

	random_num = get_random_number(minishell);
	file_name = ft_itoa(random_num);
	if (!file_name)
		return (NULL);
	while (access(file_name, F_OK) == 0)
	{
		random_num = get_random_number(minishell);
		free(file_name);
		file_name = ft_itoa(random_num);
		if (!file_name)
			return (NULL);
	}
	return (file_name);
}

int	create_heredoc_file(t_redir_file *here_doc, t_data *minishell)
{
	int	fd;

	here_doc->file_name = get_random_name(minishell);
	if (!here_doc->file_name)
		return (-1);
	here_doc->redir_type = HERE_DOC;
	fd = open(here_doc->file_name, O_WRONLY | O_APPEND | O_CREAT | 00644);
	return (fd);
}
