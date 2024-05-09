/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 21:52:16 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/09 22:40:53 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_mlxerror(void)
{
	perror(mlx_strerror(mlx_errno));
	exit(1);
}

void	mem_error(void)
{
	errno = ENOMEM;
	strerror(errno);
	exit(1);
}

void	map_cleanup(t_map **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
}

int	count_lines(char *filepath)
{
	char	*file;
	char	*line;
	int		fd;
	int		line_cnt;

	file = filepath;
	fd = open(file, O_RDONLY);
	line_cnt = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		line_cnt++;
		free(line);
	}
	if (line_cnt == 0)
	{
		ft_putstr_fd("error: Empty file\n", 1);
		exit(1);
	}
	if (close(fd) < 0)
		return (-1);
	return (line_cnt);
}

int	open_file(int argc, char *filepath)
{
	char	*file;
	int		fd;

	if (argc != 2)
	{
		errno = EINVAL;
		perror("error: Must be one file of type '.fdf' as an argument");
		exit(1);
	}
	if (ft_strendswith(filepath, ".fdf") == 0)
	{
		ft_putstr_fd("error: File must be of type '.fdf'\n", 1);
		exit(1);
	}
	file = filepath;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		errno = EBADF;
		perror("error: Could not open file");
		exit(1);
	}
	return (fd);
}
