/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/04/25 21:23:28 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libft.h"
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 720
#define HEIGHT 720
#define M_PI 3.14159265358979323846
#define BPP sizeof(int32_t)

typedef struct	fdf_pnt
{
	int		x;
	int		y;
	int		z;
	uint32_t	color;
}	fdf_pnt_t;

// Exit the program as failure.
static void	ft_error(void)
{
	perror(mlx_strerror(mlx_errno));
	// strerror(mlx_errno);
	exit(EXIT_FAILURE);
}
extern char	*get_next_line(int fd);


static void	line_draw(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx, dy;
	int sx = 0;
	int sy = 0;
	int e2 = 0;

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
// KEYHOOK FOR 'ESC' KEY TO CLOSE GRAPH WINDOW
static void	fdf_keyhooks(mlx_key_data_t keydata, void *param)
{
	mlx_t *mlx;

	mlx = (mlx_t*) param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}



int main(void)
{

	char *file = "test_maps/pyra.fdf";
	int fd = open(file, O_RDONLY);

	if (fd == -1)
	{
		printf("CAN'T OPEN FILE!\n");
		return (-1);
	}
	// // char **map_arr;
	// // map_arr = (char**) malloc(sizeof(char*));
	char *get_line1 = get_next_line(fd);
	if (!get_line1)
		return (-2);

	char **line1 = ft_split(get_line1, 32);
	if (!line1)
		return (-2);
	free(get_line1);

	char *get_line2 = get_next_line(fd);
	if (!get_line2)
		return (-2);
	char **line2 = ft_split(get_line2, 32);
	if (!line2)
		return (-2);
	free(get_line2);

	char *get_line3 = get_next_line(fd);
	if (!get_line3)
		return (-2);
	char **line3 = ft_split(get_line3, 32);
	if (!line3)
		return (-2);
	free(get_line3);

	int i = 0;

	fdf_pnt_t **map = (fdf_pnt_t**) malloc(3 * sizeof(fdf_pnt_t*));
	uint32_t color = 0xFF00FFFF;

	while (line1[i])
	{
		printf("%s", line1[i]);
		i++;
	}
	printf("\n");

	map[0] = malloc((i + 1) * sizeof(fdf_pnt_t));
	i = 0;

	while (line1[i])
	{
		map[0][i].x = i;
		map[0][i].y = 0;
		map[0][i].z = ft_atoi(line1[i]);

		uint8_t red = (color >> 24) & 0xFF;
    	red = (red >> ft_atoi(line1[i])) | (red << (8 - ft_atoi(line1[i])));
    	color = (color & 0xFF00FFFF) | (red << 16);
		map[0][i].color = color;

		free(line1[i]);
		line1[i] = NULL;
		i++;
	}

	free(line1);
	i = 0;
	while (line2[i])
	{

		printf("%s", line2[i]);
		free(line2[i]);
		line2[i] = NULL;
		i++;
	}
	printf("\n");

	free(line2);
	i = 0;
	while (line3[i])
	{

		printf("%s", line3[i]);
		free(line3[i]);
		line3[i] = NULL;
		i++;
	}
	printf("\n");

	free(line3);








	mlx_t   *mlx;

	// mlx_set_setting(MLX_MAXIMIZED, true);
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "FDF", true)))
		ft_error();

	mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error();

	line_draw(img,  WIDTH/2, HEIGHT/2, map[0][0].x, map[0][0].y, map[0][0].color);
	line_draw(img, WIDTH/2, HEIGHT/2, (WIDTH/2 + 200), HEIGHT/2  /  tan(M_PI/6), 0xFF0000FF); 	// X (RED)
	line_draw(img, WIDTH/2, HEIGHT/2, WIDTH/2 - 200, WIDTH/2 /  tan(M_PI/6), 0x00FF00FF); 	// Y (GREEN)
	line_draw(img, WIDTH/2, HEIGHT/2, WIDTH/2, (HEIGHT / 2 - 200), 0x0000FFFF); 			// Z (BLUE)


	mlx_key_hook(mlx, fdf_keyhooks, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
