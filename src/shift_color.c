/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 23:15:31 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/15 18:45:35 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

uint32_t	shift_color(uint32_t color, int z0, double error, double scale)
{
	int32_t	red;
	int32_t	green;
	int32_t	blue;
	int32_t	alpha;

	(void) error;
	red = color >> 24 & 0xFF;
	green = color >> 16 & 0xFF;
	blue = color >> 8 & 0xFF;
	alpha = color & 0xFF;
	red = (int32_t)round(red + z0 / scale * error) % 256;
	green = (int32_t)round(green + z0 / scale * error) % 256;
	blue = (int32_t)round(blue + z0 / scale * error) % 256;
	color = (red << 24) | (green << 16) | (blue << 8) | alpha;
	return (color);
}
