/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 21:11:49 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/13 01:18:39 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/fdf.h"

static bool	key_pressed(mlx_key_data_t keydata)
{
	if ((keydata.action == MLX_PRESS) || (keydata.action == MLX_REPEAT))
		return (true);
	return (false);
}

static void	all_keyhooks(mlx_key_data_t keydata, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (keydata.key == MLX_KEY_RIGHT_BRACKET && key_pressed(keydata))
		data->control->scale += 1;
	if (keydata.key == MLX_KEY_LEFT_BRACKET && key_pressed(keydata))
		data->control->scale -= 1;
	if (keydata.key == MLX_KEY_RIGHT && key_pressed(keydata))
		data->control->transpose_x += 20;
	if (keydata.key == MLX_KEY_LEFT && key_pressed(keydata))
		data->control->transpose_x -= 20;
	if (keydata.key == MLX_KEY_UP && key_pressed(keydata))
		data->control->transpose_y -= 20;
	if (keydata.key == MLX_KEY_DOWN && key_pressed(keydata))
		data->control->transpose_y += 20;
	if (keydata.key == MLX_KEY_PAGE_UP && key_pressed(keydata))
		data->control->rotate -= 0.1;
	if (keydata.key == MLX_KEY_PAGE_DOWN && key_pressed(keydata))
		data->control->rotate += 0.1;
	if (keydata.key == MLX_KEY_EQUAL && key_pressed(keydata))
		data->control->stretch += 0.5;
	if (keydata.key == MLX_KEY_MINUS && key_pressed(keydata))
		data->control->stretch -= 0.5;
	if (keydata.key == MLX_KEY_ESCAPE && key_pressed(keydata))
		mlx_close_window(data->mlx);
}

static void	render_bg(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < HEIGHT)
	{
		j = 0;
		while (j < WIDTH)
		{
			mlx_put_pixel(data->img, j, i, 0x222222FF);
			j++;
		}
		i++;
	}
}

static void	render_map(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map[i] != NULL)
	{
		j = 0;
		while (j < data->elem_cnt - 1)
		{
			draw_line(data, data->map[i][j], data->map[i][j + 1]);
			j++;
		}
		i++;
	}
	j = 0;
	while (j < data->elem_cnt)
	{
		i = 0;
		while (data->map[i + 1] != NULL)
		{
			draw_line(data, data->map[i][j], data->map[i + 1][j]);
			i++;
		}
		j++;
	}
}

void	render_control(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	render_bg((t_data *)data);
	render_map((t_data *)data);
	mlx_key_hook(data->mlx, all_keyhooks, data);
}
