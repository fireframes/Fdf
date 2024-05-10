/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/10 23:47:00 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_params(t_control *control, int line_cnt, int elem_cnt)
{
	control->scale = (int)(((sqrt(WIDTH / elem_cnt)
					+ sqrt(HEIGHT / line_cnt))) / (WIDTH / HEIGHT * 1.0));
	control->transpose_x = 0;
	control->transpose_y = 0;
	control->rotate = 1.0;
	control->stretch = 5.33;
}

void	fill_map(t_map *map, char **line, int i)
{
	int	j;

	j = 0;
	while (line[j])
	{
		map[j].x = j;
		map[j].y = i;
		map[j].z = ft_atoi(line[j]);
		map[j].color = COLOR;
		free(line[j]);
		j++;
	}
}

void	parse_map(int fd, t_data *data)
{
	char	*getline;
	char	**line;
	int		i;

	i = 0;
	while (1)
	{
		getline = get_next_line(fd);
		if (!getline)
			break ;
		line = ft_split(getline, 32);
		if (!line)
			mem_error();
		free(getline);
		data->elem_cnt = 0;
		while (line[data->elem_cnt])
			data->elem_cnt++;
		data->map[i] = (t_map *)malloc((data->elem_cnt) * sizeof(t_map));
		if (!data->map[i])
			mem_error();
		fill_map(data->map[i], line, i);
		free(line);
		i++;
	}
	data->map[i] = NULL;
}

int	main(int argc, char *argv[])
{
	int		fd;
	int		line_cnt;
	t_data	data;

	fd = open_file(argc, argv[1]);
	line_cnt = count_lines(argv[1]);
	data.map = (t_map **)malloc((line_cnt + 1) * sizeof(t_map *));
	parse_map(fd, &data);
	if (close(fd) < 0)
		return (-1);
	data.mlx = mlx_init(WIDTH, HEIGHT, "FDF", true);
	if (!data.mlx)
		ft_mlxerror();
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		ft_mlxerror();
	data.control = (t_control *)malloc(sizeof(t_control));
	init_params(data.control, line_cnt, data.elem_cnt);
	mlx_loop_hook(data.mlx, render_engine, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	map_cleanup(data.map);
	free(data.control);
	return (0);
}
