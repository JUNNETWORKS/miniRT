#include "minirt.h"

int	initialize_world(t_world *world)
{
	world->mlx = mlx_init();
	world->screen_width = 800;
	world->screen_height = 800;
	world->win = mlx_new_window(world->mlx,
		world->screen_width, world->screen_height, "miniRT");
	world->img.img = mlx_new_image(world->mlx,
		world->screen_width, world->screen_height);
	world->img.addr = mlx_get_data_addr(world->img.img,
		&world->img.bits_per_pixel, &world->img.line_length, &world->img.endian);
	world->img.width = world->screen_width;
	world->img.height = world->screen_height;
	return (0);
}

int	main_loop(t_world *world)
{
	clear_img(&world->img);
	mlx_put_image_to_window(world->mlx, world->win, world->img.img, 0, 0);
	return (0);
}

int main()
{
	t_world world;
	initialize_world(&world);
	mlx_loop_hook(world.mlx, &main_loop, &world);
	mlx_loop(world.mlx);
}
