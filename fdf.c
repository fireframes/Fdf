/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/02 22:37:33 by mmaksimo         ###   ########.fr       */
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
	double angle = M_PI / 4.0 * control->rotate;
	int x_tmp;

	int shift_x = WIDTH/2 + control->transpose_x;
	int shift_y = HEIGHT/4 + control->transpose_y;

	x_tmp = (int) round((*x - *y) * cos(angle));
	*y = (int) round((*x + *y) * sin(angle) - *z) + shift_y;
	*x = x_tmp + shift_x;

}
/*
static void	crop_outside(int *x, int *y, double dx_i, double dy_i)
{

	double a = dy_i / dx_i; // SLOPE
	double b = *y - *x * a; // INTERCEPT
	
	// printf("a:	%lf\t", a);
	// printf("b:	%lf\n", b);
	
	unsigned int yi = 0;
	unsigned int xi = 0;
	
	// printf("before x:	%d\t", *x);
	// printf("before y:	%d\n", *y);
	
	if (*y > HEIGHT)
	{
		yi = HEIGHT - 1;
		xi = (unsigned int) round((yi - b) / a);
	}
	else if (*y < 0)
		xi = (unsigned int) round((yi - b) / a);
	
	if (*x > WIDTH)
	{
		xi = WIDTH - 1;
		yi = (unsigned int) round(a * xi + b);
	}
	else if (*x < 0)
		yi = (unsigned int) round(a * xi + b);
	
	*x = xi;
	*y = yi;
	
}
*/
static void crop_outside(int *_x, int *_y, double dx_i, double dy_i)
{
    double a = dy_i / dx_i; // SLOPE
    double b = *_y - *_x * a; // INTERCEPT

    unsigned int xi = *_x;
    unsigned int yi = *_y;

    // Clamp x and y values within the valid range
    if (*_x < 0)
        xi = 0;
    else if (*_x > WIDTH)
        xi = WIDTH - 1;

    if (*_y < 0)
        yi = 0;
    else if (*_y > HEIGHT)
        yi = HEIGHT - 1;

    // Calculate new coordinates if outside the valid range
    if (xi == 0 || xi == WIDTH - 1)
        yi = (unsigned int)round(a * xi + b);
    else if (yi == 0 || yi == HEIGHT - 1)
        xi = (unsigned int)round((yi - b) / a);

    *_x = xi;
    *_y = yi;
}

static void	line_draw(mlx_image_t *img, int x0, int y0, int z0, int x1, int y1, int z1, uint32_t color, ctrl_param_t *control)
{
	double dx, dy;
	double sx = 0;
	double sy = 0;
	double e2 = 0;

	iso_point(&x0, &y0, &z0, control);
	iso_point(&x1, &y1, &z1, control);

	// printf("POINT_0  x0 | y0 : %d\t%d\n", x0, y0);
	// printf("POINT_1  x1 | y1 : %d\t%d\n", x1, y1);

	double dx_i;
	double dy_i;

	dx_i = x1 - x0;
	dy_i = y1 - y0;

	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);

	if (y0 > HEIGHT || y0 < 0)
	{
		// printf("POINT_0  x0 | y0 : %d\t%d\n", x0, y0);
		crop_outside(&x0, &y0, dx_i, dy_i);
		// printf("CROPPED  x0 | y0 : %d\t%d\n", x0, y0);  // MAYBE I NEED TO PASS THE (x0, y0) values as well

	}
	if (y1 > HEIGHT || y1 < 0)
	{
		// printf("POINT_1  x1 | y1 : %d\t%d\n", x1, y1);
		crop_outside(&x1, &y1, dx_i, dy_i);
		// printf("CROPPED  x1 | y1 : %d\t%d\n", x1, y1);

	}

	// BRESENHAM'S LINE DRAWING ALGORITHM

	// ~ NEED TO ADD FILTER TO CUT OFFMAP VALUES! ~



	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;

  	double error = dx + dy;

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
		while (j < data->elem - 1) // INSTEAD OF SUBTRACTING ONE POSITION MAYBE IT WOULD BE BETTER TO CLEAN THE TRAILING NULS WITH FT_STRTRIM?
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

	
	data.map = (map_pt_t**) malloc((lines_cnt + 1) * sizeof(map_pt_t*));
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
			return (-3);
		free(get_line);
		get_line = NULL;

		data.elem = 0;				// SEPARATE FUNCTION COUNT ELEM
		while (line[data.elem])
			data.elem++;

		data.map[i] = (map_pt_t*) malloc((data.elem + 1) * sizeof(map_pt_t));

		j = 0;
		while (line[j])
		{
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
	data.control->scale = (int)(((sqrt(WIDTH / data.elem) + sqrt(HEIGHT / lines_cnt))) / (WIDTH / HEIGHT * 1.0));
	printf("CALCULATED SCALE: %d\n", data.control->scale);
	
	data.control->transpose_x = 0;
	data.control->transpose_y = 0;
	data.control->rotate = 1.0;

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

// - NORM CODE !
// - COLOR GRADIENT !!
// - CRASHES AND LEAKS !!!
// -- REBEL POINTS
// --- SMOOTHER LINES
// ---- CENTRALED ANCHOR POINT
