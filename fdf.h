/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 01:23:34 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/08 20:00:32 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libft.h"
#include "MLX42/include/MLX42/MLX42.h"

#ifndef FDF_H
# define FDF_H

# define WIDTH 1600
# define HEIGHT 900
# define M_PI 3.14159265358979323846
# define COLOR (uint32_t) 0x55AAFFFF
# define IS_KEY_PRESSED(keydata) ((keydata.action == MLX_PRESS) \
		|| (keydata.action == MLX_REPEAT))

typedef struct	map
{
	int			x;
	int			y;
	int			z;
	uint32_t	color;
}	map_t;

typedef struct ctrl_param
{
	int		scale;
	int		transpose_x;
	int		transpose_y;
	float	rotate;
	float	stretch;
}	ctrl_param_t;

typedef struct data
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	map_t			**map;
	ctrl_param_t	*control;
	int				elem;
}	data_t;

#endif