#include "minirt.h"

int		initialize_world(t_world *world)
{
	world->mlx = mlx_init();
	world->screen_width = 511;
	world->screen_height = 511;
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

int		initialize_objects(t_world *world)
{
	t_object *object;

	if (!(object = sphere_init(vec3_init(3, 0, 25), 1)) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(2, 0, 20), 1)) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(1, 0, 15), 1)) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(0, 0, 10), 1)) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = plane_init(vec3_init(0, 0, 0), vec3_init(1,1,1))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	return (0);
}

int	raytracing(t_world *world)
{
	// 視点位置を表すベクトル
	t_vec3 camera_vec;
	camera_vec = vec3_init(0, 0, -5);  // スクリーンの少し手前な感じ

	// 点光源(light)
	t_vec3 light_vec;
	light_vec = vec3_init(-5, 5, -5);
	for (double x = 0; x < world->screen_width; x++){
		for (double y = 0; y < world->screen_height; y++){
			// スクリーン座標からワールド座標への変換
			// x,yは[-1,1]へ変換する
			// スクリーン上の点の三次元空間における位置を計算する
			t_vec3 screen_vec;
			screen_vec = vec3_init(2 * x / world->screen_width - 1.0, 2 * y / world->screen_height - 1.0, 0);

			// レイ(光線)
			t_ray ray;
			ray.start = camera_vec;
			ray.direction = vec3_normalize(vec3_sub(screen_vec, camera_vec));

			// もっと交点距離の短いオブジェクトを取得する

			if (sphere_has_intersection(ray, sphere))
			{
				t_intersection intersection = calc_sphere_intersection(ray, sphere);
				// 交点までの距離がマイナスということはスクリーンより後ろにあるということ
				if (intersection.distance < 0)
					continue;

				// 光の入射ベクトルの計算
				t_vec3 l;
				l = vec3_normalize(vec3_sub(light_vec, intersection.position));

				double ray_deg = vec3_dot(intersection.normal, l);
				// 法線ベクトルと入射ベクトルの成す角がπ/2を超えた時, 光は裏側から当たっているので反射は起きない
				if (ray_deg < 0)
					ray_deg = 0;

				// 拡散反射光R_dの計算
				// 光源の強度I_i
				t_fcolor I_i = init_fcolor(1.0, 1.0, 1.0);
				// 拡散反射係数k_d
				t_fcolor k_d = init_fcolor(0.69, 0.69, 0.0);
				// 拡散反射光の放射輝度R_d
				t_fcolor R_d = fcolor_mult_scalar(fcolor_mult(k_d, I_i), ray_deg);

				// 環境光の強度
				t_fcolor I_a = init_fcolor(0.1, 0.1, 0.1);
				// 環境光反射係数
				t_fcolor k_a = init_fcolor(0.01, 0.01, 0.01);
				// 環境光R_a
				t_fcolor R_a = fcolor_mult(k_a, I_a);

				// 鏡面反射光R_sの計算
				// 鏡面反射係数k_s
				t_fcolor k_s = init_fcolor(0.3, 0.3, 0.3);
				// 光沢度α
				double alpha = 8;
				// 視線ベクトルの逆ベクトル
				t_vec3 v = vec3_mult(ray.direction, -1);
				// 入射光の正反射ベクトル
				t_vec3 r = vec3_sub(vec3_mult(vec3_mult(intersection.normal, vec3_dot(intersection.normal, l)), 2), l);
				// 鏡面反射光の放射強度R_s
				t_fcolor R_s = fcolor_mult_scalar(fcolor_mult(k_s, I_i), pow(vec3_dot(v, r), alpha));
				if (vec3_dot(v, r) < 0)
					R_s = init_fcolor(0, 0, 0);

				// 最終的な輝度  (環境光 + 拡散反射光 + 鏡面反射光)
				t_fcolor R_r = fcolor_add(fcolor_add(R_a, R_d), R_s);
				my_mlx_pixel_put(&world->img, x, world->screen_height - y - 1, fcolor2hex(R_r));
			}
			else
			{
				my_mlx_pixel_put(&world->img, x, world->screen_height - y - 1, rgb2hex(0, 0, 255));
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
	initialize_objects(&world);
	mlx_loop_hook(world.mlx, &main_loop, &world);
	mlx_loop(world.mlx);
}
