/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrade <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:39:05 by randrade          #+#    #+#             */
/*   Updated: 2024/09/06 14:33:11 by randrade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*new_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	new_line = NULL;
	if (!buffer[0])
	{
		if (read(fd, buffer, BUFFER_SIZE) < 0)
			return (NULL);
	}
	while (buffer[0])
	{
		new_line = ft_strjoin_gnl(new_line, buffer);
		if (!new_line)
			return (NULL);
		if (ft_cleanbuf_gnl(buffer) == 1)
			break ;
		if (read(fd, buffer, BUFFER_SIZE) < 0)
			return (free(new_line), NULL);
	}
	return (new_line);
}
