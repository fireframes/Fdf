/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 01:11:37 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/10 23:48:55 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static uint32_t	shift_color(uint32_t color, int z0, double error, double scale)
{
	int32_t	red;
	int32_t	green;
	int32_t	blue;
	int32_t	alpha;

	// (void) error;
	red = color >> 24 & 0xFF;
	green = color >> 16 & 0xFF;
	blue = color >> 8 & 0xFF;
	alpha = color & 0xFF;
	red = (int32_t)round(red + z0 / scale * error) % 256;
	green = (int32_t)round(green + z0 / scale * error) % 256;
	blue = (int32_t)round(blue + z0 / scale * error) % 256;
	color = (red << 24) | (green << 16) | (blue << 8) | alpha;
	// printf("%lf\n", fabs(error));
	return (color);
}

static void	count_dir_error(t_bres_lda *lda, t_map *p0, t_map *p1)
{
	lda->sx = -1;
	if (p0->x < p1->x)
		lda->sx = 1;
	lda->sy = -1;
	if (p0->y < p1->y)
		lda->sy = 1;
	lda->error = abs(p1->x - p0->x) - abs(p1->y - p0->y);
}

void	bresenham_lda(t_map p0, t_map p1, mlx_image_t *img, t_control *control)
{
	t_bres_lda	lda;

	count_dir_error(&lda, &p0, &p1);
	while (1)
	{
		mlx_put_pixel(img, p0.x, p0.y,
					shift_color(COLOR, p0.z, lda.error, control->stretch));
		if (p0.x == p1.x && p0.y == p1.y)
			break ;
		lda.e2 = 2 * lda.error;
		if (lda.e2 >= (-abs(p1.y - p0.y)))
		{
			if (p0.x == p1.x)
				break ;
			lda.error = lda.error - abs(p1.y - p0.y);
			p0.x = p0.x + lda.sx;
		}
		if (lda.e2 <= (-abs(p1.y - p0.y)))
		{
			if (p0.y == p1.y)
				break ;
			lda.error = lda.error + abs(p1.x - p0.x);
			p0.y = p0.y + lda.sy;
		}
	}
}
