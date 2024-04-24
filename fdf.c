/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 16:43:59 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/04/23 23:42:44 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MLX42/include/MLX42/MLX42.h"

#define WIDTH 720
#define HEIGHT 720

#define BPP sizeof(int32_t)

typedef struct fdf_pnt
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
}   fdf_pnt_t;

// Exit the program as failure.
static void ft_error(void)
{
	perror(mlx_strerror(mlx_errno));
	// strerror(mlx_errno);
	exit(EXIT_FAILURE);
}


static void line_draw(mlx_image_t *img, int x0, int y0, int x1, int y1)
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
		mlx_put_pixel(img, x0, y0, 0xFF00FFFF);
        
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
	


	

	// mlx_put_pixel(img, x1, y1, 0xFF00FFFF);

			
}

void    fdf_keyhooks(mlx_key_data_t keydata, void *param)
{
	mlx_t *mlx;
	
	mlx = (mlx_t*) param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

int main(void)
{
	mlx_t   *mlx;

	// mlx_set_setting(MLX_MAXIMIZED, true);
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "FDF", true)))
		ft_error();
	
	mlx_image_t *img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error();
	// mlx_image_t *square = mlx_new_image(mlx, 128, 128);
	// memset(square->pixels, 255, square->width * square->height * BPP);
	
	// Bakcground
	line_draw(img, 0, 0, 100, 150);
	line_draw(img, 0, 0, 120, 510);
	line_draw(img, 100, 100, 0, 0);
	line_draw(img, 325, 123, 0, 0);

	// White square
	// mlx_image_to_window(mlx, square, 0, 0);
	
	mlx_key_hook(mlx, fdf_keyhooks, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
