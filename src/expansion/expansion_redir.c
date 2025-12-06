/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgiancol <jgiancol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 19:15:00 by jgiancol          #+#    #+#             */
/*   Updated: 2025/12/06 19:14:08 by jgiancol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_redir_filename(char *filename, t_data *data)
{
	char	*expanded;

	if (!filename)
		return (NULL);
	expanded = expand_variables(filename, data);
	return (expanded);
}
