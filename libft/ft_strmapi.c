/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dde-sou2 <danilo.bleach12@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:19:31 by dde-sou2          #+#    #+#             */
/*   Updated: 2025/07/25 22:28:51 by dde-sou2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	str_size;
	size_t	i;
	char	*new_str;

	str_size = ft_strlen(s);
	new_str = ft_calloc(str_size + 1, sizeof(*new_str));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < str_size)
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	return (new_str);
}
