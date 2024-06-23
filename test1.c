/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhongu <fhongu@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 00:57:49 by fhongu            #+#    #+#             */
/*   Updated: 2024/06/23 12:09:01 by fhongu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <mlx.h>
#include <ft_printf.h>
#include <get_next_line.h>
#include <stdlib.h>
#include <sys/time.h>
#include "src/include/so_long.h"
#include <fcntl.h>
#define TILE_SIZE 64

typedef struct s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_vector2 {
	int	x;
	int	y;
}	t_vector2;

typedef struct s_vars {
	void	*mlx;
	void	*window;
	t_data	*img;
	t_data	*character;
	int		key_pressed;
	time_t	time_pressed;
	time_t	time_elapsed;
	int		i;
	int		j;
	int		colorstate;
	int		r;
	int		g;
	int		b;
	int			player_x;
	int			player_y;
	t_vector2	player_pos;
	time_t	last_frame;
	int		x_offset;
	int		y_offset;
	int		keyspamming;
	int		nplayers;
	int		nexits;
	int		ncollectibles;
	char	*map;
}	t_vars;

typedef enum {less_red, more_green, less_blue, more_red, less_green, more_blue} colorstates;

void	my_pixel_put(t_data *img, int x, int y, int color);
int		key_pressed(int keycode, t_vars *vars);
int		key_released(int keycode, t_vars *vars);
int		mouse_enter(int x, int y, t_vars *vars);
int		mouse_exit(int x, int y, t_vars *vars);

int	destroy_button(t_vars *vars)
{
	ft_printf("bye!\n");
	mlx_do_key_autorepeaton(vars->mlx);
	exit(0);
}

int	mouse_motion(int x, int y, t_vars *vars)
{
	ft_printf("mouse x: %d, mouse y: %d\n", x, y);
	return (0);
}

int	render_frames(t_vars *vars)
{
	/*
	while (vars->i < 100)
	{
		vars->j = 0;
		while (vars->j < 100)
		{			
			my_pixel_put(vars->img, vars->i + vars->x_offset,
				vars->j + vars->y_offset,
				ft_trgb(0, vars->r, vars->g, vars->b));
			vars->j++;
		}
		vars->i++;
	}
	vars->i = 0;
	vars->j = 0;
	time_t tm = ft_millis();
	int diff = (int) (tm - vars->last_frame) >= (unsigned int) 30;
	if (vars->r > 0 && diff)
		vars->r--;
	if (vars->g < 255 && diff)
		vars->g++;
	if (vars->b > 0 && diff)
		vars->b--;
	if (vars->x_offset < 400 && diff)
		vars->x_offset++;
	if (vars->y_offset > 50 && diff)
		vars->y_offset--;
	mlx_put_image_to_window(vars->mlx, vars->window, vars->img->img, 0, 0);
	if (diff)
		vars->last_frame = tm;
	*/
	unsigned int	diff;
	time_t			tm;

	tm = ft_millis();
	diff = (unsigned int) tm - vars->last_frame >= (unsigned int) 30;
	if (diff)
	{
		vars->i = 0;
		while (vars->i < 1280)
		{
			vars->j = 0;
			while (vars->j < 768)
			{
				my_pixel_put(vars->img, vars->i + vars->x_offset,
					vars->j + vars->y_offset,
					ft_trgb(0, vars->r, vars->g, vars->b));
				vars->j++;
			}
			vars->i++;
		}
		switch (vars->colorstate)
		{
			case less_red:
				vars->r--;
				if (vars->r == 0)
					vars->colorstate = more_green;
				break;
			case more_green:
				vars->g++;
				if (vars->g == 255)
					vars->colorstate = less_blue;
				break;
			case less_blue:
				vars->b--;
				if (vars->b == 0)
					vars->colorstate = more_red;
				break;
			case more_red:
				vars->r++;
				if (vars->r == 255)
					vars->colorstate = less_green;
				break;
			case less_green:
				vars->g--;
				if (vars->g == 0)
					vars->colorstate = more_blue;
				break;
			case more_blue:
				vars->b++;
				if (vars->b == 255)
					vars->colorstate = less_red;
				break;
		}
		vars->last_frame = tm;
		mlx_put_image_to_window(vars->mlx, vars->window, vars->img->img, 0, 0);
		mlx_put_image_to_window(vars->mlx, vars->window, vars->character->img,
		TILE_SIZE * vars->player_x, TILE_SIZE * vars->player_y);
	}
	return (0);
}

int	can_be_solved(t_vars *vars)
{
	int	solvable;

	solvable = 1;
	/* Recursive alg starting from player pos, that fills everything with P
	 * And counts the collectibles and exits. When everything has been filled,
	 * it makes sure all the exits and collectibles have been accounted for.
	 * It has a while loop with all four directions and checks all four of them,
	 * then calls itself for each direction and after calling itself every time,
	 * it replaces non wall tiles with P. In the end every collectible should
	 * have been accounted for and the exit should also be reachable.
	 * Test thoroughly.
	 */
	return (solvable);
}

int	is_valid(char *filename, t_vars *vars)
{
	int		fd;
	int		line_length;
	int		nb_lines;
	int		is_rect;
	int		i;
	int		j;
	char	*next_line;
	char	*map;

	fd = open(filename, O_RDONLY);
	is_rect = 1;
	if (fd != -1)
	{
		map = malloc(sizeof (char));
		next_line = get_next_line(fd);
		nb_lines = 0;
		line_length = ft_strlen(next_line) - ft_char_count(next_line, '\n');
		while (next_line)
		{
			if (ft_strlen(next_line) - ft_char_count(next_line, '\n')
				!= line_length)
			{
				ft_free((void **) &next_line);
				is_rect = 0;
				ft_printf("Error\nThe map isn't rectangular.\n");
				break ;
			}
			i = 0;
			while (next_line[i])
			{
				if (next_line[i] == 'P')
				{
					vars->player_pos.x = i;
					vars->player_pos.y = nb_lines;
					vars->nplayers++;
				}
				else if (next_line[i] == 'E')
					vars->nexits++;
				else if (next_line[i] == 'C')
					vars->ncollectibles++;
				i++;
			}
			map = ft_append(map, next_line, 0, 1);
			next_line = get_next_line(fd);
			nb_lines++;
		}
	}
	else
	{
		ft_printf("Error\nFile '%s' doesn't exist or is unreadable.\n",
			filename);
		return (0);
	}
	ft_printf(map);
	i = 0;
	line_length++;
	while (i < nb_lines)
	{
		if (i == 0 || i == nb_lines - 1)
		{
			j = 0;
			while (j < line_length - 1)
			{
				if (map[i * line_length + j] != '1')
				{
					is_rect = 0;
					ft_printf("Error\nThe map has a hole at %d:%d.\n", i, j);
					break ;
				}
				j++;
			}
			if (!is_rect)
				break ;
		}
		else
		{
			if (map[i * line_length] != '1'
				|| map[(i + 1) * line_length - 2] != '1')
			{
				is_rect = 0;
				ft_printf("Error\nThe map has a hole in line %d\n", i);
				break ;
			}
		}
		i++;
	}
	vars->map = map;
	if (vars->nplayers != 1)
		ft_printf("Error\nThere is more than one player position.\n");
	else if (vars->nexits != 1)
		ft_printf("Error\nThere is more than one exit.\n");
	else if (vars->ncollectibles == 0)
		ft_printf("Error\nThere are no collectibles in this map.\n");
	return (is_rect && !(vars->nplayers != 1 || vars->nexits != 1
			|| vars->ncollectibles == 0) && can_be_solved(vars));
}

int	check_file(char *filepath, t_vars *vars)
{
	int		len;
	char	*filename;
	char	*filetype;

	vars->nplayers = 0;
	vars->nexits = 0;
	vars->ncollectibles = 0;
	filename = ft_strrchr(filepath, '/');
	if (!filename)
		filename = filepath;
	else
		filename++;
	len = ft_strlen(filename);
	if (len < 5
		|| ft_strncmp(".ber", ft_substr(filename, len - 4, len), 4) != 0)
	{
		if (len < 5)
			ft_printf("Error\nFilename too short or incorrect file format.\n");
		else
			ft_printf("Error\nIncorrect file format.");
		return (0);
	}
	return (is_valid(filepath, vars));
}

int	check_params(int argc, char **argv, t_vars *vars)
{
	int	valid;

	valid = 1;
	if (argc != 2)
	{
		ft_printf("Error\nInvalid number of arguments. %s",
			"Usage: so_long <filepath>\n");
		valid = 0;
	}
	else if (!check_file(argv[1], vars))
		valid = 0;
	return (valid);
}

int	main(int argc, char **argv)
{
	t_vars	vars;
	t_data	img;
	t_data	character;
	int		char_width;
	int		char_height;
	char	*addres;

	if (!check_params(argc, argv, &vars))
		return (0);
	vars.key_pressed = 0;
	vars.mlx = mlx_init();
	vars.window = mlx_new_window(vars.mlx, 1280, 768, "My super duper window!");
	img.img = mlx_new_image(vars.mlx, 1280, 768);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
			&img.endian);
	vars.img = &img;
	addres = "./assets/vampire.xpm";
	character.img = mlx_xpm_file_to_image(vars.mlx, addres,
			&char_width, &char_height);
	character.addr = mlx_get_data_addr(character.img, &character.bits_per_pixel,
			&character.line_length, &character.endian);
	vars.character = &character;
	vars.colorstate = 0;
	vars.r = 255;
	vars.g = 0;
	vars.b = 255;
	vars.x_offset = 0;
	vars.y_offset = 0;
	vars.i = 0;
	vars.player_x = 0;
	vars.player_y = 0;
	vars.keyspamming = 0;
	vars.time_elapsed = 0;
	vars.last_frame = 0;
	mlx_do_key_autorepeatoff(vars.mlx);
	mlx_hook(vars.window, 2, 1L << 0, key_pressed, (void *) &vars);
	mlx_hook(vars.window, 3, 1L << 1, key_released, (void *) &vars);
	mlx_hook(vars.window, 7, (1L << 4), mouse_enter, (void *) &vars);
	mlx_hook(vars.window, 8, (1L << 5), mouse_exit, (void *) &vars);
	mlx_hook(vars.window, DestroyNotify, 0L, destroy_button, (void *) &vars);
	mlx_hook(vars.window, MotionNotify, PointerMotionMask,
		mouse_motion, (void *) &vars);
	mlx_loop_hook(vars.mlx, render_frames, (void *) &vars);
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
	if (keycode != vars->key_pressed)
		ft_printf("key pressed: %d\n", keycode);
	vars->key_pressed = keycode;
	vars->time_pressed = ft_millis();
	return (0);
}

int	time_elapsed(t_vars *vars)
{
	return (ft_millis() - vars->time_pressed);
}

int	key_released(int keycode, t_vars *vars)
{
	int	this_time;

	if (keycode == XK_Escape)
	{
		mlx_do_key_autorepeaton(vars->mlx);
		mlx_destroy_window(vars->mlx, vars->window);
		exit(0);
	}
	else if ((keycode == XK_w || keycode == XK_Up) && vars->player_y > 0)
		vars->player_y--;
	else if ((keycode == XK_s || keycode == XK_Down) && vars->player_y < 11)
		vars->player_y++;
	else if ((keycode == XK_a || keycode == XK_Left) && vars->player_x > 0)
		vars->player_x--;
	else if ((keycode == XK_d || keycode == XK_Right) && vars->player_x < 19)
		vars->player_x++;
	if (keycode == vars->key_pressed)
		ft_printf("player pos: (%d, %d)\n", vars->player_x, vars->player_y);
	return (0);
}

int	mouse_enter(int x, int y, t_vars *vars)
{
	ft_printf("mouse entered!\n");
	return (0);
}

int	mouse_exit(int x, int y, t_vars *vars)
{
	ft_printf("mouse left!\n");
	return (0);
}
