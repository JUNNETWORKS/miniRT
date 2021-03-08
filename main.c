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
	// 視点位置を表すベクトル
	t_vec3 camera_vec;
	camera_vec = vec3_init(0, 0, -5);  // スクリーンの少し手前な感じ

	// 球の中心座標
	t_vec3 square_vec;
	square_vec = vec3_init(0, 0, 5);  // スクリーンの少し奥な感じ
	double square_r = 1;  // 半径

	for (double y = 0; y < world->screen_height; y++){
		for (double x = 0; x < world->screen_width; x++){
			// スクリーン座標からワールド座標への変換
			// x,yは[-1,1]へ変換する
			// スクリーン上の点の三次元空間における位置を計算する
			t_vec3 screen_vec;
			screen_vec = vec3_init(2 * x / world->screen_width - 1.0, 2 * y / world->screen_height - 1.0, 0);

			// 方向ベクトル
			t_vec3 dir_vec;
			dir_vec = vec3_normalize(vec3_sub(screen_vec, camera_vec));

			// レイが球に当たったか計算する
			double a = vec3_mag(dir_vec) * vec3_mag(dir_vec);
			double b = 2 * vec3_dot(camera_vec, dir_vec);
			double c = vec3_mag(camera_vec) * vec3_mag(camera_vec) - square_r * square_r;
			// 判別式
			double d = b * b - 4 * a * c;
			if (d >= 0)
			{
				my_mlx_pixel_put(&world->img, x, y, rgb2hex(255, 0, 0));
			}
			else
			{
				my_mlx_pixel_put(&world->img, x, y, rgb2hex(0, 0, 255));
			}
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
