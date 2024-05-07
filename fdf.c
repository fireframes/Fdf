/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/08 01:31:44 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

extern char	*get_next_line(int fd);

static void	ft_mlxerror(void)
{
	perror(mlx_strerror(mlx_errno));
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
	color = (color & r_mask) | (red << 24);
	color = (color & g_mask) | (green << 16);
	color = (color & b_mask) | (blue << 8);
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
	// color = color * (z1 - z0) / 0.01;
	// color = shift_color(color, (z1 - z0));

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
	if (keydata.key == MLX_KEY_RIGHT_BRACKET && IS_KEY_PRESSED(keydata))
		data->control->scale += 1;
	if (keydata.key == MLX_KEY_LEFT_BRACKET && IS_KEY_PRESSED(keydata))
		data->control->scale -= 1;
	if (keydata.key == MLX_KEY_RIGHT && IS_KEY_PRESSED(keydata))
		data->control->transpose_x += 20;
	if (keydata.key == MLX_KEY_LEFT && IS_KEY_PRESSED(keydata))
		data->control->transpose_x -= 20;
	if (keydata.key == MLX_KEY_UP && IS_KEY_PRESSED(keydata))
		data->control->transpose_y -= 20;
	if (keydata.key == MLX_KEY_DOWN && IS_KEY_PRESSED(keydata))
		data->control->transpose_y += 20;
	if (keydata.key == MLX_KEY_PAGE_UP && IS_KEY_PRESSED(keydata))
		data->control->rotate -= 0.1;
	if (keydata.key == MLX_KEY_PAGE_DOWN && IS_KEY_PRESSED(keydata))
		data->control->rotate += 0.1;
	if (keydata.key == MLX_KEY_EQUAL && IS_KEY_PRESSED(keydata))
		data->control->stretch += 0.5;
	if (keydata.key == MLX_KEY_MINUS && IS_KEY_PRESSED(keydata))
		data->control->stretch -= 0.5;
	if (keydata.key == MLX_KEY_ESCAPE && IS_KEY_PRESSED(keydata))
		mlx_close_window(data->mlx);
}

static void	render_bg(map_data_t *data)
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
}

static int	render_map(map_data_t *data)
{
	int	i;
	int	j;

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
	map_data_t	*data;

	data = (map_data_t*) param;
	render_bg((map_data_t*) data);
	render_map((map_data_t*) data);
	mlx_key_hook(data->mlx, all_keyhooks, data);
}

int ft_strendswith(const char *str, const char *end)
{
	const char	*end0;

	if (!str || !end)
		return (0);
	end0 = &str[ft_strlen(str) - ft_strlen(end)];
	if (ft_strncmp(end0, end, ft_strlen(end)) == 0)
		return (1);
	return (0);
}

int count_lines(char *filepath)
{
	char	*file;
	char	*line;
	int		fd;
	int		line_cnt;

	file = filepath;
	fd = open(file, O_RDONLY);
	line_cnt = 0;
	while ((line = get_next_line(fd)))
	{
		line_cnt++;
		free(line);
	}
	if (line_cnt == 0)
	{
		ft_putstr_fd("error: Empty file\n", 1);
		exit(1);
	}
	if (close(fd) < 0)
	{
		ft_putstr_fd("error: Could not close the file properly\n", 1);
		exit(1);
	}
	return (line_cnt);
}

int open_file(int argc, char *filepath)
{
	char	*file;
	int		fd;
	
	if (argc != 2)
	{
		errno = EINVAL;
		perror("error: Program takes a path to file of type '.fdf' as argument");
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

int main(int argc, char *argv[])
{
	int	fd;
	int	line_cnt;

	fd = open_file(argc, argv[1]);
	line_cnt = count_lines(argv[1]);
	
	map_data_t data;
	
	data.map = (map_pt_t**) malloc((line_cnt + 1) * sizeof(map_pt_t*));
	uint32_t color = 0x55AAFFFF;

	int i;
	int j;
	char *getline;
	char **line;

	i = 0;
	while ((getline = get_next_line(fd)))
	{
		line = ft_split(getline, 32);
		if (!line)
			return (-3);
		free(getline); // cleanup
		getline = NULL;

		data.elem = 0;
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

			free(line[j]); // cleanup
			line[j] = NULL;

			j++;
		}
		free(line); // cleanup
		line = NULL;
		i++;
	}
	data.map[i] = NULL;

	// CLOSE FILE

	if (close(fd) < 0)
	{
		perror("Couldn't close the map file properly.");
		exit(1);
	}



	// SETUP MLX INSATANCE AND DRAW WIREFRAME


	if (!(data.mlx = mlx_init(WIDTH, HEIGHT, "FDF", true)))
		ft_mlxerror();

	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		ft_mlxerror();


	// CONTROLLING PARAMS (SCALE / TRANSPOSE / ROTATE)

	data.control = (ctrl_param_t *) malloc(sizeof(ctrl_param_t));
	data.control->scale = (int)(((sqrt(WIDTH / data.elem) + sqrt(HEIGHT / line_cnt))) / (WIDTH / HEIGHT * 1.0));

	data.control->transpose_x = 0;
	data.control->transpose_y = 0;
	data.control->rotate = 1.0;
	data.control->stretch = 8.0;

	mlx_loop_hook(data.mlx, render_map_wrapper, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);

// cleanup function

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

	return (0);
}


// TODO:

// - NORM CODE !
// - COLOR PARSING AND GRADIENT !!
// - MAKEFILE !!
// - UNDERSTAND YOUR ALGORITHMS))
// - perror or ft_printf ?

// --- CHECK IF .FDF FILE IS FORMATED RIGHT
	// ???
// --- SMOOTHER LINES
	// ???
// ---- CENTERED ANCHOR POINT
	// ???


	// printf("LINE COUNT:  %d\nELEMS COUNT: %d\n", line_cnt, data.elem);
	// printf("CALCULATED SCALE: %d\n", data.control->scale);


// colors

	// printf("red: %x\n", red);
	// printf("green: %x\n", green);
	// printf("blue: %x\n", blue);
	// printf("in color: %x\n", color);
