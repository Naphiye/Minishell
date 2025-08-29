/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsalomon <fsalomon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 09:23:40 by anastruc          #+#    #+#             */
/*   Updated: 2024/07/26 16:43:56 by fsalomon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(long nb)
{
	int	len;

	len = 0;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	else if (nb == 0)
		len++;
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(unsigned long n)
{
	char			*str;
	unsigned long	nb;
	int				i;

	nb = n;
	i = ft_numlen(nb);
	str = malloc(sizeof(char) * (i + 2));
	if (!str)
		return (NULL);
	str[i] = 0;
	if (nb == 0)
		str[0] = 48;
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	while (nb > 0 && i > 0)
	{
		i--;
		str[i] = (nb % 10) + 48;
		nb /= 10;
	}
	return (str);
}

/*
int	main (void)
{
	int	n;

	n = 25;
	__builtin_printf("%p\n", ft_itoa(n));
	return(0);
}
*/
