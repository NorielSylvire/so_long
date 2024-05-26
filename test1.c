/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhongu <fhongu@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 00:57:49 by fhongu            #+#    #+#             */
/*   Updated: 2024/05/26 12:55:11 by fhongu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <mlx.h>
#include <ft_printf.h>
#include <sys/time.h>
#include "src/include/so_long.h"

typedef struct s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_vars {
	void	*mlx;
	void	*window;
	int		key_pressed;
	time_t	time_pressed;
	int		i;
	int		j;
	int		x_offset;
	int		y_offset;
}	t_vars;

void	my_pixel_put(t_data *img, int x, int y, int color);
int		key_pressed(int keycode, t_vars *vars);
int		key_released(int keycode, t_vars *vars);

int	main(void)
{
	t_vars	vars;
	t_data	img;

	vars.key_pressed = 0;
	vars.mlx = mlx_init();
	vars.window = mlx_new_window(vars.mlx, 600, 400, "My super duper window!");
	img.img = mlx_new_image(vars.mlx, 600, 400);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
			&img.endian);
	vars.x_offset = 300;
	vars.y_offset = 200;
	vars.i = 0;
	while (vars.i < 100)
	{
		vars.j = 0;
		while (vars.j < 100)
		{			
			my_pixel_put(&img, vars.i + vars.x_offset, vars.j + vars.y_offset,
				0x00FF0075);
			vars.j++;
		}
		vars.i++;
	}
	mlx_put_image_to_window(vars.mlx, vars.window, img.img, 0, 0);
	mlx_hook(vars.window, 2, 1L << 0, key_pressed, (void *) &vars);
	mlx_hook(vars.window, 3, 1L << 1, key_released, (void *) &vars);
	mlx_loop(vars.mlx);
	return (0);
}

void	my_pixel_put(t_data *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	key_pressed(int keycode, t_vars *vars)
{
	ft_printf("key pressed: %d", keycode);
	vars->key_pressed = keycode;
	vars->time_pressed = ft_millis();
	return (0);
}

int	key_released(int keycode, t_vars *vars)
{
	if (keycode == XK_Escape)
	{
		mlx_destroy_window(vars->mlx, vars->window);
		exit(0);
	}
	return (0);
}
