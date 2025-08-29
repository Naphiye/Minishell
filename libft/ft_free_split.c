/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:12:10 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/09 14:25:57 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_free_split(char **dest)
{
	size_t	i;

	i = 0;
	if (dest)
	{
		if (dest[i])
		{
			while (dest[i])
			{
				free(dest[i]);
				dest[i] = NULL;
				i++;
			}
		}
		free(dest);
		dest = NULL;
	}
	return (NULL);
}
