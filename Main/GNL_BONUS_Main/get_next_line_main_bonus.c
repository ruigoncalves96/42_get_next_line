/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_main_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrade <randrade@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:59:36 by randrade          #+#    #+#             */
/*   Updated: 2024/09/12 13:06:05 by randrade         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../get_next_line_bonus.h"

# define FILE_NBR 5

int	main(void)
{
	int	fd[FILE_NBR];
	char	*line;
	int	i;
	int	finished_files;

	line = NULL;
	fd[0] = open("GNL_BONUS_Main/text.txt", O_RDONLY);
	fd[1] = open("GNL_BONUS_Main/1char.txt", O_RDONLY);
	fd[2] = open("GNL_BONUS_Main/nl.txt", O_RDONLY);
	fd[3] = open("GNL_BONUS_Main/no_nl.txt", O_RDONLY);
	fd[4] = open("GNL_BONUS_Main/empty.txt", O_RDONLY);

	finished_files = 0;
	i = 0;
	while (i < FILE_NBR)
	{
		if (fd[i++] < 0)
		{
			while (--i > 0)
				close(fd[i]);
			return (1);
		}
	}

	while (finished_files < FILE_NBR)
	{
		i = 0;
		while (i < FILE_NBR)
		{
			if (fd[i] != -1)
			{
				line = get_next_line(fd[i]);
				if (line)
				{
					printf("FD[%d] - %s\n", i + 1, line);
					free(line);
				}
				else
				{
					close(fd[i]);
					fd[i] = -1;
					finished_files++;
				}
			}
			i++;
		}

	}
	return (0);
}
