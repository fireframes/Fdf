/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 01:34:43 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/08 01:36:31 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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