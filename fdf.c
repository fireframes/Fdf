/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/04/30 23:13:38 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/input.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libft.h"
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 3500
#define HEIGHT 1500
// #define SHIFT WIDTH/2
#define M_PI 3.14159265358979323846
// #define BPP sizeof(int32_t)


typedef struct	map_pnt
{
	int			x;
	int			y;
	int			z;
	uint32_t	color;
}	map_pt_t;

typedef struct ctrl_param
{
	int	scale;
	int	transpose_x;
	int	transpose_y;
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

static void	iso_point(int *x, int *y, int *z)
{
	double angle = M_PI / 4.0;
	int x_tmp;

	int shift_x = WIDTH/2;
	int shift_y = HEIGHT/2;

	x_tmp = (int) round((*x - *y) * cos(angle));
	*y = (int) round((*x + *y) * sin(angle) - *z) + shift_y;
	*x = x_tmp + shift_x;

}
static void	crop_outside(int *x, int *y, double dx_i, double dy_i)
{

	double a = dy_i / dx_i; // SLOPE
	double b = *y - *x * a; // INTERCEPT
	int yi = HEIGHT - 1;
	int xi = (int) ((double)yi - b) / a;
	if (*y > HEIGHT)
	{
		*x = xi;
		*y = yi;
	}
}

static void	line_draw(mlx_image_t *img, int x0, int y0, int z0, int x1, int y1, int z1, uint32_t color)
{
	int dx, dy;
	int sx = 0;
	int sy = 0;
	int e2 = 0;


	// printf("ORIGINAL x0 | y0 | z0 : %d\t%d\t%d\n", x0, y0, z0);
	// printf("ORIGINAL x1 | y1 | z1 : %d\t%d\t%d\n", x1, y1, z1);


	iso_point(&x0, &y0, &z0);
	iso_point(&x1, &y1, &z1);

	// printf("POINT_0  x0 | y0 : %d\t%d\n", x0, y0);
	// printf("POINT_1  x1 | y1 : %d\t%d\n", x1, y1);

	double dx_i = x1 - x0;
	double dy_i = y1 - y0;

	if (y0 > HEIGHT)
	{
		printf("POINT_0  x0 | y0 : %d\t%d\n", x0, y0);
		crop_outside(&x0, &y0, dx_i, dy_i);
		printf("CROPPED  x0 | y0 : %d\t%d\n", x0, y0);  // MAYBE I NEED TO PASS THE (x0, y0) values as well

	}
	if (y1 > HEIGHT)
	{
		printf("POINT_1  x1 | y1 : %d\t%d\n", x1, y1);
		crop_outside(&x1, &y1, dx_i, dy_i);
		printf("CROPPED  x1 | y1 : %d\t%d\n", x1, y1);

	}

	// BRESENHAM'S LINE DRAWING ALGORITHM

	// ~ NEED TO ADD FILTER TO CUT OFFMAP VALUES! ~

	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);

	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;

  	int error = dx + dy;

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

	printf("scale -- %d\n", data->control->scale);

	if (keydata.key == MLX_KEY_UP && keydata.action == MLX_PRESS)
		data->control->scale += 10;
	if (keydata.key == MLX_KEY_DOWN && keydata.action == MLX_PRESS)
		data->control->scale -= 10;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(data->mlx);
}

static int	render_map(map_data_t *data)
{
	int	i;
	int	j;


	//RENDER BG

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
		while (j < data->elem - 1) // INSTEAD OF SUBTRACTING ONE POSITION MAYBE IT WOULD BE BETTER TO CLEAN THE TRAILING NULS WITH FT_STRTRIM?
		{
			line_draw(data->img, data->control->scale * data->map[i][j].x, data->control->scale * data->map[i][j].y,
					data->map[i][j].z, data->control->scale * data->map[i][j+1].x, data->control->scale * data->map[i][j+1].y, data->map[i][j+1].z, data->map[i][j].color);

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
			line_draw(data->img, data->control->scale * data->map[i][j].x, data->control->scale * data->map[i][j].y, data->map[i][j].z,
					data->control->scale * data->map[i + 1][j].x, data->control->scale * data->map[i+1][j].y, data->map[i+1][j].z, data->map[i][j].color);

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
		perror("Couldn't open file properly.");
		exit(1);
	}



	// CHECK FILE_prime FORMAT (ALL ROWS HAVE SAME NUMBER OF ELEMENTS)
	// COUNT LINES IN A FILE_prime

	char *file_second = argv[1];
	int fd_2 = open(file_second, O_RDONLY);

	int lines_cnt;

	lines_cnt = 0;
	while (get_next_line(fd_2))
		lines_cnt++;

	if (close(fd_2) < 0)
	{
		perror("Couldn't close the map file properly.");
		exit(1);
	}


	// GET LINE AND SPLIT
	// POPULATE MAP ARRAY

	map_data_t data;


	data.map = (map_pt_t**) malloc(lines_cnt * sizeof(map_pt_t*));
	uint32_t color = 0x55AAFFFF;

	int i;
	int j;
	char *get_line = NULL;
	char **line;

	i = 0;
	get_line = get_next_line(fd_1);
	if (!get_line)
	{
		free(get_line);
		get_line = NULL;
	}
	while (get_line)
	{
		line = ft_split(get_line, 32);
		if (!line)
		{
			printf("BREAKS ON ft_split!\n");
			return (-3);
		}

		free(get_line);
		get_line = NULL;

		data.elem = 0;				// SEPARATE FUNCTION COUNT ELEM
		while (line[data.elem])
			data.elem++;

		data.map[i] = (map_pt_t*) malloc((data.elem + 1) * sizeof(map_pt_t));

		j = 0;
		while (line[j])
		{
			// printf("i: %d | j: %d\n", j, i);

			data.map[i][j].x = j;
			data.map[i][j].y = i;
			data.map[i][j].z = ft_atoi(line[j]) * 3;

			// SHIFT COLORS ACCORDING TO Z VALUE
			// uint8_t red = (color >> 24) & 0xFF;
			int shift_amount = data.map[i][j].z % 8; // Ensure shift_amount is between 0 and 7
			// red = (red >> shift_amount) | (red << (8 - shift_amount));
			// color = (color & 0x55AAFFFF) | (red << 24);
			data.map[i][j].color = color >> abs(shift_amount/2);


			free(line[j]);
			line[j] = NULL;

			j++;
		}
		free(line);
		line = NULL;

		get_line = get_next_line(fd_1);
		if (!get_line)
		{
			free(get_line);
			get_line = NULL;
			break ;
		}

		i++;
	}
	data.map[i] = NULL; // NULL FOR LAST ELEM OF MAP ARRAY

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
	data.control->scale = 30;


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

	return (EXIT_SUCCESS);
}


/// PROBLEMS AND SUGGESTIONS ///

// -- MEMORY LEAKING BCOF GNL!
