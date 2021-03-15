#include "minirt.h"

int		initialize_world(t_world *world)
{
	world->mlx = mlx_init();
	world->objects = NULL;
	world->lights = NULL;
	return (0);
}

int		configure_window(t_world *world)
{
	int		max_width;
	int		max_height;

	mlx_get_screen_size(world->mlx, &max_width, &max_height);
	world->screen_width = MIN(world->screen_width, max_width);
	world->screen_height = MIN(world->screen_height, max_height);
	if (!(world->win = mlx_new_window(world->mlx,
		world->screen_width, world->screen_height, "miniRT")))
		return (put_and_return_err("creating window is failed"));
	return (0);
}

int		configure_screen(t_world *world, bool has_window)
{
	if (has_window && configure_window(world) == ERROR)
		return (ERROR);
	world->img.img = mlx_new_image(world->mlx,
		world->screen_width, world->screen_height);
	world->img.addr = mlx_get_data_addr(world->img.img,
		&world->img.bits_per_pixel, &world->img.line_length, &world->img.endian);
	world->img.width = world->screen_width;
	world->img.height = world->screen_height;
	return (0);
}

