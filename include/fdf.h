/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 01:23:34 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/13 01:20:57 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "libft.h"
# include "../MLX42/include/MLX42/MLX42.h"

# define WIDTH 1600
# define HEIGHT 1000
# define M_PI 3.14159265358979323846
# define COLOR 0x249AE3FF

typedef struct s_map
{
	int	x;
	int	y;
	int	z;
	int	color;
}	t_map;

typedef struct s_bres_lda
{
	double	sx;
	double	sy;
	double	error;
	double	e2;
}	t_bres_lda;

typedef struct s_control
{
	int		scale;
	int		transpose_x;
	int		transpose_y;
	float	rotate;
	float	stretch;
}	t_control;

typedef struct s_data
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_map		**map;
	t_control	*control;
	int			elem_cnt;
}	t_data;

int			open_file(int argc, char *filepath);
int			count_lines(char *filepath);
void		mem_error(void);
char		*get_next_line(int fd);
void		ft_mlxerror(void);
void		render_control(void *param);
void		bresenham(t_map p0, t_map p1, mlx_image_t *img, t_control *control);
void		draw_line(t_data *data, t_map map_p0, t_map map_p1);
uint32_t	shift_color(uint32_t color, int z0, double error, double scale);
void		map_cleanup(t_map **map);

#endif
