/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_main.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrade <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:12:02 by randrade          #+#    #+#             */
/*   Updated: 2024/09/06 13:46:26 by randrade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	int	fd;
	char	*line;

	line = NULL;
	fd = open("GNL_Main/text.txt", O_RDONLY);
//	fd = open("GNL_Main/1char.txt", O_RDONLY);
//	fd = open("GNL_Main/nl.txt", O_RDONLY);
//	fd = open("GNL_Main/no_nl.txt", O_RDONLY);
//	fd = open("GNL_Main/empty.txt", O_RDONLY);
//	fd = open("GNL_Main/words.txt", O_RDONLY);
//	fd = 0;
//	fd = 1;
	if (fd < 0)
		return (1);

	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	while (line)
	{
		line = get_next_line(fd);
		if (line)
			printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
