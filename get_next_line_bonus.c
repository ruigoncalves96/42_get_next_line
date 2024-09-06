/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrade <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:39:05 by randrade          #+#    #+#             */
/*   Updated: 2024/09/06 14:32:43 by randrade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	static char	buffer[1024][BUFFER_SIZE + 1];
	char		*new_line;

	if (fd < 0 || fd > 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	new_line = NULL;
	if (!buffer[fd][0])
	{
		if (read(fd, buffer[fd], BUFFER_SIZE) < 0)
			return (NULL);
	}
	while (buffer[fd][0])
	{
		new_line = ft_strjoin_gnl(new_line, buffer[fd]);
		if (!new_line)
			return (NULL);
		if (ft_cleanbuf_gnl(buffer[fd]) == 1)
			break ;
		if (read(fd, buffer[fd], BUFFER_SIZE) < 0)
			return (free(new_line), NULL);
	}
	return (new_line);
}
