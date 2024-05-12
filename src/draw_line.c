/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 01:34:43 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/13 01:20:18 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static int	iso_point(int *x, int *y, int *z, t_control *control)
{
	double	angle;
	int		x_tmp;
	int		shift_x;
	int		shift_y;

	shift_x = WIDTH / 2 + control->transpose_x;
	shift_y = HEIGHT / 4 + control->transpose_y;
	angle = M_PI / 4 * control->rotate;
	*x *= control->scale;
	*y *= control->scale;
	x_tmp = round((*x - *y) * cos(angle));
	*y = round((*x + *y) * sin(angle) - (*z * control->stretch)) + shift_y;
	*x = x_tmp + shift_x;
	if (*x >= WIDTH || *x < 0)
		return (0);
	else if (*y >= HEIGHT || *y < 0)
		return (0);
	return (1);
}

void	draw_line(t_data *data, t_map map_p0, t_map map_p1)
{
	mlx_image_t	*img;
	t_control	*control;
	t_map		p0;
	t_map		p1;

	p0 = map_p0;
	p1 = map_p1;
	img = data->img;
	control = data->control;
	if (!iso_point(&p0.x, &p0.y, &p0.z, control))
		return ;
	if (!iso_point(&p1.x, &p1.y, &p1.z, control))
		return ;
	bresenham(p0, p1, img, control);
}
