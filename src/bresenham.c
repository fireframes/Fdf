/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 01:11:37 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/13 01:20:24 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

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

void	bresenham(t_map p0, t_map p1, mlx_image_t *img, t_control *control)
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
