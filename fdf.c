/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/04/29 01:12:13 by mmaksimo         ###   ########.fr       */
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
#define HEIGHT 1600
#define M_PI 3.14159265358979323846
#define BPP sizeof(int32_t)

typedef struct	fdf_pnt
{
	int		x;
	int		y;
	int		z;
	uint32_t	color;
}	fdf_pnt_t;

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
	
	fdf_pnt_t **map = (fdf_pnt_t**) malloc(lines_cnt * sizeof(fdf_pnt_t*));
	uint32_t color = 0x55AAFFFF;

	int i;
	int j;
	int elem;
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
		
		elem = 0;
		while (line[elem])
			elem++;
		
		map[i] = (fdf_pnt_t*) malloc((elem + 1) * sizeof(fdf_pnt_t));
		
		j = 0;
		while (line[j])
		{
			// printf("i: %d | j: %d\n", j, i);

			map[i][j].x = j;
			map[i][j].y = i;
			map[i][j].z = ft_atoi(line[j]);
			
			// SHIFT COLORS ACCORDING TO Z VALUE
			// uint8_t red = (color >> 24) & 0xFF;
			int shift_amount = map[i][j].z % 8; // Ensure shift_amount is between 0 and 7
			// red = (red >> shift_amount) | (red << (8 - shift_amount));
			// color = (color & 0x55AAFFFF) | (red << 24);
			map[i][j].color = color >> abs(shift_amount/2);
			

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
	map[i] = NULL; // NULL FOR LAST ELEM OF MAP ARRAY
	
	// CLOSE FILE
	
	if (close(fd_1) < 0)
	{
		perror("Couldn't close the map file properly.");
		exit(1);
	}
	
	printf("LINE COUNT:  %d\nELEMS COUNT: %d\n", lines_cnt, elem);

	// SETUP MLX INSATANCE AND DRAW WIREFRAME

	mlx_t   *mlx;
	
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "FDF", true)))
		ft_error();

	mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 50, 50) < 0))
		ft_error();
	
	int scale = 50;
	
	i = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while (j < elem)
		{
			line_draw(img, scale * j, scale * i, scale * map[i][j+1].x, scale * map[i][j+1].y, map[i][j].color);
			printf("%d %d %d %x\n", map[i][j].x, map[i][j].y, map[i][j].z, map[i][j].color);
			j++;
		}
		i++;
	}

	mlx_key_hook(mlx, fdf_keyhooks, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	
	i = 0;
	while (map[i])
	{
		free(map[i]);
		map[i] = NULL;
		i++;
	}
	free(map);
	map = NULL;
	
	return (EXIT_SUCCESS);
}


/// PROBLEMS AND SUGGESTIONS ///

// -- MEMORY LEAKING BCOF GNL!