/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 01:23:34 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/08 01:30:57 by mmaksimo         ###   ########.fr       */
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
# define IS_KEY_PRESSED(keydata) ((keydata.action == MLX_PRESS) \
		|| (keydata.action == MLX_REPEAT))

typedef struct	map_pnt
{
	int			x;
	int			y;
	int			z;
	uint32_t	color;
}	map_pt_t;

typedef struct ctrl_param
{
	int		scale;
	int		transpose_x;
	int		transpose_y;
	float	rotate;
	float	stretch;
}	ctrl_param_t;

typedef struct map_data
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	map_pt_t		**map;
	ctrl_param_t	*control;
	int				elem;
}	map_data_t;

#endif