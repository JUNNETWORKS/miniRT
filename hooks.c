#include "minirt.h"

int 			key_press_hook(int keycode, t_world *world)
{
	// printf("Pressed Key Code: %d\n", keycode);
	if (keycode == KEY_q || keycode == KEY_esc){
		mlx_destroy_window(world->mlx, world->win);
		exit(0);
	}
	if (keycode == KEY_lallow)
		if (world->cameras)
			world->cameras = world->cameras->next;
	if (keycode == KEY_rallow)
		if (world->cameras)
			world->cameras = world->cameras->prev;
	return (0);
}

int	exit_world(t_world *world)
{
	mlx_destroy_window(world->mlx, world->win);
	exit(0);
}

