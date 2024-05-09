/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crop_outside.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaksimo <mmaksimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:43:51 by mmaksimo          #+#    #+#             */
/*   Updated: 2024/05/04 15:18:32 by mmaksimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// static void	crop_outside(int *x, int *y, double dx_i, double dy_i)
// {

// 	double a = dy_i / dx_i; // SLOPE
// 	double b = *y - *x * a; // INTERCEPT

// 	// printf("a:	%lf\t", a);
// 	// printf("b:	%lf\n", b);

// 	unsigned int yi = 0;
// 	unsigned int xi = 0;

// 	// printf("before x:	%d\t", *x);
// 	// printf("before y:	%d\n", *y);

// 	if (*y > HEIGHT)
// 	{
// 		yi = HEIGHT - 1;
// 		xi = (unsigned int) round((yi - b) / a);
// 	}
// 	else if (*y < 0)
// 		xi = (unsigned int) round((yi - b) / a);

// 	if (*x > WIDTH)
// 	{
// 		xi = WIDTH - 1;
// 		yi = (unsigned int) round(a * xi + b);
// 	}
// 	else if (*x < 0)
// 		yi = (unsigned int) round(a * xi + b);

// 	*x = xi;
// 	*y = yi;

// }

// static void crop_outside(int *_x, int *_y, double dx_i, double dy_i)
// {
// 	double a = dy_i / dx_i; // SLOPE
// 	double b = *_y - *_x * a; // INTERCEPT

// 	int xi = *_x;
// 	int yi = *_y;

// 	// Clamp x and y values within the valid range

// 	// if (*_x > WIDTH)
// 	// 	xi = WIDTH - 1;
// 	// if (*_y > HEIGHT)
// 	// 	yi = HEIGHT - 1;

// 	// Calculate new coordinates if outside the valid range
// 	if (xi == 0 || xi == WIDTH - 1)
// 		yi = (int)round(a * xi + b);
// 	else if (yi == 0 || yi == HEIGHT - 1)
// 		xi = (int)round((yi - b) / a);

// 	*_x = xi;
// 	*_y = yi;
// }

	// ~ FILTER TO CUT OFFMAP VALUES ~


	// double dx_i;
	// double dy_i;

	// dx_i = x1 - x0;
	// dy_i = y1 - y0;

	// crop_outside(&x0, &y0, dx_i, dy_i);
	// printf("CROPPED  x0 | y0 : %d\t%d\n", x0, y0);
	// crop_outside(&x1, &y1, dx_i, dy_i);
	// printf("CROPPED  x1 | y1 : %d\t%d\n", x1, y1);

	// printf("CLAMPED  x0 | y0 : %d\t%d\n", x0, y0);
	// printf("CLAMPED  x1 | y1 : %d\t%d\n", x1, y1);

	// if (y0 > HEIGHT || y0 < 0)
	// {
	// 	// return ;
	// 	// printf("POINT_0  x0 | y0 : %d\t%d\n", x0, y0);
	// 	crop_outside(&x0, &y0, dx_i, dy_i);
	// 	printf("CROPPED  x0 | y0 : %d\t%d\n", x0, y0);

	// }
	// if (y1 > HEIGHT || y1 < 0)
	// {
	// 	// return ;
	// 	// printf("POINT_1  x1 | y1 : %d\t%d\n", x1, y1);
	// 	crop_outside(&x1, &y1, dx_i, dy_i);
	// 	printf("CROPPED  x1 | y1 : %d\t%d\n", x1, y1);
	// }
