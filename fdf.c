/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/07 00:52:25 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libft.h"
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 1600
#define HEIGHT 900
#define M_PI 3.14159265358979323846

typedef struct	map_pnt
{
	int			x;
	int			y;
	int			z;
	uint32_t	color;
}	map_pt_t;

typedef struct ctrl_param
{
	int		scale;
	int		transpose_x;
	int		transpose_y;
	float	rotate;
	float	stretch;
}	ctrl_param_t;

typedef struct map_data
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	map_pt_t		**map;
	ctrl_param_t	*control;
	int				elem;
}	map_data_t;

extern char	*get_next_line(int fd);

static void	ft_error(void)
{
	perror(mlx_strerror(mlx_errno));
	// strerror(mlx_errno);
	exit(EXIT_FAILURE);
}

static void	iso_point(int *x, int *y, int *z, ctrl_param_t *control)
{
	double	angle;
	int		x_tmp;
	int		shift_x;
	int		shift_y;

	shift_x = WIDTH / 2 + control->transpose_x;
 	shift_y = HEIGHT / 4 + control->transpose_y;
	angle = M_PI / 4 * control->rotate;
	x_tmp = (int) round((*x - *y) * cos(angle));
	*y = (int) round((*x + *y) * sin(angle) - (*z * control->stretch)) + shift_y;
	*x = x_tmp + shift_x;
}
uint32_t shift_color(uint32_t color, int z0)
{

	uint32_t r_mask = 0x00FFFFFF;
	uint32_t g_mask = 0xFF00FFFF;
	uint32_t b_mask = 0xFFFF00FF;
	// int dz = abs(z1 - z0);
	int red = color >> 24;
	int green = color >> 16 & 0xFF;
	int blue = color >> 8 & 0xFF;

	red = red + (z0 / 2) % 8;
	green = green + (z0 / 2) % 8;
	blue = blue + (z0 / 2) % 8;

	// printf("red: %x\n", red);
	// printf("green: %x\n", green);
	// printf("blue: %x\n", blue);
	// printf("in color: %x\n", color);

	color = (color & r_mask) | (red << 24);
	color = (color & g_mask) | (green << 16);
	color = (color & b_mask) | (blue << 8);

	// printf("out color: %x\n", color);
	return (color);
}

static void	line_draw(mlx_image_t *img, int x0, int y0, int z0, int x1, int y1, int z1, uint32_t color, ctrl_param_t *control)
{
	double	dx;
	double	dy;
	double	sx;
	double	sy;
	double	error;
	double	e2;
	
	iso_point(&x0, &y0, &z0, control);
	iso_point(&x1, &y1, &z1, control);
	

	// CARPET SETUP	
	color = color * (z1 - z0) / 0.01;	
	color = shift_color(color, (z1 - z0));

	// ANDREW'S METHOD - JUST RETURNING WHAT IS OUT OF BOUNDS
	if (x0 >= WIDTH || x1 >= WIDTH)
		return ;
	else if (y0 >= HEIGHT || y1 >= HEIGHT)
		return ;
	else if (x0 < 0 || x1 < 0)
		return ;
	else if (y0 < 0 || y1 < 0)
		return ;

	// BRESENHAM'S LINE DRAWING ALGORITHM

	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);

	sx = -1;
	if (x0 < x1)
		sx = 1;
	sy = -1;
	if (y0 < y1)
		sy = 1;
	error = dx + dy;

while (1)
	{
		mlx_put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * error;
		if (e2 >= dy)
		{
			if (x0 == x1)
				break ;
			error = error + dy;
			x0 = x0 + sx;
		}
		if (e2 <= dy)
		{
			if (y0 == y1)
				break ;
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

static void	all_keyhooks(mlx_key_data_t keydata, void *param)
{
	map_data_t	*data;

	data = (map_data_t*) param;
	if (keydata.key == MLX_KEY_RIGHT_BRACKET && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->scale += 1;
	if (keydata.key == MLX_KEY_LEFT_BRACKET && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->scale -= 1;
	if (keydata.key == MLX_KEY_RIGHT && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->transpose_x += 10;
	if (keydata.key == MLX_KEY_LEFT && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->transpose_x -= 10;
	if (keydata.key == MLX_KEY_UP && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->transpose_y -= 10;
	if (keydata.key == MLX_KEY_DOWN && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->transpose_y += 10;
	if (keydata.key == MLX_KEY_PAGE_UP && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->rotate -= 0.1;
	if (keydata.key == MLX_KEY_PAGE_DOWN && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->rotate += 0.1;
	if (keydata.key == MLX_KEY_EQUAL && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->stretch += 0.5;
	if (keydata.key == MLX_KEY_MINUS && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		data->control->stretch -= 0.5;
	if (keydata.key == MLX_KEY_ESCAPE && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT))
		mlx_close_window(data->mlx);
}

static int	render_map(map_data_t *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			mlx_put_pixel(data->img, j, i, 0x222222FF);
			j++;
		}
		i++;
	}
	i = 0;
	while (data->map[i] != NULL)
	{
		j = 0;
		while (j < data->elem - 1)
		{
			line_draw(data->img, data->control->scale * data->map[i][j].x, data->control->scale * data->map[i][j].y,
					data->map[i][j].z, data->control->scale * data->map[i][j+1].x, data->control->scale * data->map[i][j+1].y,
					data->map[i][j+1].z, data->map[i][j].color, data->control);
			j++;
		}
		i++;
	}
	j = 0;
	while (j < data->elem)
	{
		i = 0;
		while (data->map[i + 1] != NULL)
		{
			line_draw(data->img, data->control->scale * data->map[i][j].x, data->control->scale * data->map[i][j].y,
					data->map[i][j].z, data->control->scale * data->map[i + 1][j].x, data->control->scale * data->map[i+1][j].y,
					data->map[i+1][j].z, data->map[i][j].color, data->control);
			i++;
		}
		j++;
	}
	return (0);
}

void render_map_wrapper(void *param)
{
	map_data_t *data = (map_data_t*) param;

	render_map((map_data_t*) data);
	mlx_key_hook(data->mlx, all_keyhooks, data);
}

int main(int argc, char *argv[])
{

	if (argc != 2)
	{
		perror("Usage: ./fdf *.fdf");
		exit(1);
	}

	char *file_prime = argv[1];
	int fd_1 = open(file_prime, O_RDONLY);

	if (fd_1 == -1)
	{
		perror("Couldn't open file properly");
		exit(1);
	}


	// CHECK FILE_prime FORMAT (ALL ROWS HAVE SAME NUMBER OF ELEMENTS)
	// COUNT LINES IN A FILE_prime

	char *file_second = argv[1];
	char *tmp_getline;
	int fd_2 = open(file_second, O_RDONLY);

	int lines_cnt;

	lines_cnt = 0;
	while ((tmp_getline = get_next_line(fd_2)))
	{
		lines_cnt++;
		free(tmp_getline);
		// tmp_getline = NULL;
	}
	if (close(fd_2) < 0)
	{
		perror("Couldn't close the map file properly.");
		exit(1);
	}


	// GET LINE AND SPLIT
	// POPULATE MAP ARRAY

	map_data_t data;

	data.map = (map_pt_t**) malloc((lines_cnt + 1) * sizeof(map_pt_t*));
	uint32_t color = 0x55AAFFFF;

	int i;
	int j;
	char *getline;
	char **line;

	i = 0;
	while ((getline = get_next_line(fd_1)))
	{
		line = ft_split(getline, 32);
		if (!line)
			return (-3);
		free(getline);
		getline = NULL;

		data.elem = 0;				// SEPARATE FUNCTION COUNT ELEM
		while (line[data.elem])
			data.elem++;

		if (!(data.map[i] = (map_pt_t*) malloc((data.elem) * sizeof(map_pt_t))))
		{
			perror("Could not allocate memory for map during parsing:");
			exit(1);
		}
		j = 0;
		while (line[j])
		{
			data.map[i][j].x = j;
			data.map[i][j].y = i;
			data.map[i][j].z = ft_atoi(line[j]);
			data.map[i][j].color = color;

			free(line[j]);
			line[j] = NULL;

			j++;
		}
		free(line);
		line = NULL;
		i++;
	}
	data.map[i] = NULL;

	// CLOSE FILE

	if (close(fd_1) < 0)
	{
		perror("Couldn't close the map file properly.");
		exit(1);
	}

	printf("LINE COUNT:  %d\nELEMS COUNT: %d\n", lines_cnt, data.elem);

	// SETUP MLX INSATANCE AND DRAW WIREFRAME


	if (!(data.mlx = mlx_init(WIDTH, HEIGHT, "FDF", true)))
		ft_error();

	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		ft_error();


	// CONTROLLING PARAMS (SCALE / TRANSPOSE / ROTATE)

	data.control = (ctrl_param_t *) malloc(sizeof(ctrl_param_t));
	data.control->scale = (int)(((sqrt(WIDTH / data.elem) + sqrt(HEIGHT / lines_cnt))) / (WIDTH / HEIGHT * 1.0));
	printf("CALCULATED SCALE: %d\n", data.control->scale);

	data.control->transpose_x = 0;
	data.control->transpose_y = 0;
	data.control->rotate = 1.0;
	data.control->stretch = 8.0;

	mlx_loop_hook(data.mlx, render_map_wrapper, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);

	i = 0;
	while (data.map[i])
	{
		free(data.map[i]);
		data.map[i] = NULL;
		i++;
	}
	free(data.map);
	data.map = NULL;
	free(data.control);
	data.control = NULL;

	return (EXIT_SUCCESS);
}


// TODO:

// - MAKE TOP VIEW
// - NORM CODE !
// - COLOR PARSING AND GRADIENT !!
// - MAKEFILE !!
// - UNDERSTAND YOUR ALGORITHMS))


// + SOME STRANGE POINTS AND LINES IN DIFF FILES
	// because of spaces in the end of lines
// + CRASHES AND LEAKS !!!
	// DONE!
// +- REBEL POINTS
	// fixed by just skipping outborder point recalculating
	// (maybe come up with original cropping solurion later)

// --- SMOOTHER LINES
	// ???
// ---- CENTERED ANCHOR POINT
	// ???

/* PROBLEMS OPENING FILES:

- passing maps dir crashes the program -- make program to take only <.fdf> files

*/
