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

int	raytracing(t_world *world)
{
	for (int y = 0; y < world->screen_height; y++){
		for (int x = 0; x < world->screen_width; x++){
			// スクリーン座標からワールド座標への変換
			// x,yは[-1,1]へ変換する
			// スクリーン上の点の三次元空間における位置を計算する
			t_vec3 screen_vec;
			screen_vec.x = 2 * x / world->screen_width - 1.0;
			screen_vec.y = 2 * y / world->screen_height - 1.0;
			screen_vec.z = 0;
		}
	}
	return (0);
}

int	main_loop(t_world *world)
{
	clear_img(&world->img);
	raytracing(world);
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
