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
	world->objects = NULL;
	world->lights = NULL;
	return (0);
}

int		initialize_objects(t_world *world)
{
	t_object *object;

	if (!(object = sphere_init(vec3_init(3, 0, 25), 1,
			material_init(fcolor_init(0.01, 0.01, 0.01),
							fcolor_init(0.69, 0.0, 0.0),
							fcolor_init(0.3, 0.3, 0.3)))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(2, 0, 20), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.0, 0.69, 0.0),
								fcolor_init(0.3, 0.3, 0.3)))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(1, 0, 15), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.0, 0.0, 0.69),
								fcolor_init(0.3, 0.3, 0.3)))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(0, 0, 10), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.0, 0.69, 0.69),
								fcolor_init(0.3, 0.3, 0.3)))) ||
			!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(-1, 0, 5), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.69, 0.0, 0.69),
								fcolor_init(0.3, 0.3, 0.3)))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = plane_init(vec3_init(0, -1, 0), vec3_init(0, 1, 0), 
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.69, 0.69, 0.69),
								fcolor_init(0.3, 0.3, 0.3)))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	// 光源
	t_light *light;
	if (!(light = light_init(vec3_init(-5, 5, -5), fcolor_init(0.5, 0.5, 0.5))) ||
		!(ft_lstadd_back_new(&world->lights, light)))
		return (put_and_return_err("failed malloc light"));
	if (!(light = light_init(vec3_init(-5, 0, -5), fcolor_init(0.5, 0.5, 0.5))) ||
		!(ft_lstadd_back_new(&world->lights, light)))
		return (put_and_return_err("failed malloc light"));
	if (!(light = light_init(vec3_init(5, 20, -5), fcolor_init(0.5, 0.5, 0.5))) ||
		!(ft_lstadd_back_new(&world->lights, light)))
		return (put_and_return_err("failed malloc light"));
	// 環境光
	world->ambient_intensity = fcolor_init(0.1, 0.1, 0.1);
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
			screen_vec = vec3_init(2 * x / (world->screen_width - 1) - 1, -2 * y / (world->screen_height - 1) + 1, 0);

			// レイ(光線)
			t_ray ray;
			ray.start = camera_vec;
			ray.direction = vec3_normalize(vec3_sub(screen_vec, camera_vec));

			// もっと交点距離の短いオブジェクトを取得する
			t_object *nearest_object_ptr;
			nearest_object_ptr = get_nearest_object(world, ray);
			if (nearest_object_ptr)
			{
				t_intersection intersection = calc_intersection(ray, *nearest_object_ptr);
				// 交点までの距離がマイナスということはスクリーンより後ろにあるということ
				if (intersection.distance < 0)
					continue;

				t_list *current_light = world->lights;
				// 拡散反射光(Diffuse) + 鏡面反射光(Specular)
				t_fcolor R_ds = fcolor_init(0, 0, 0);
				while (current_light)
				{
					// 物体表面の反射輝度(拡散反射光と鏡面反射光)の計算
					t_fcolor current_R_ds = calc_lighting_Rds(ray,
														*nearest_object_ptr,
														intersection,
														*(t_light*)current_light->content);
					R_ds = fcolor_add(R_ds, current_R_ds);
					current_light = current_light->next;
				}
				// 環境光の強度
				t_fcolor I_a = world->ambient_intensity;
				// 環境光反射係数
				t_fcolor k_a = nearest_object_ptr->material.kAmb;
				// 環境光R_a
				t_fcolor R_a = fcolor_mult(k_a, I_a);
				// 最終的な輝度  環境光 + (拡散反射光(Diffuse) + 鏡面反射光(Specular))
				t_fcolor R_r = fcolor_add(R_a, R_ds);

				my_mlx_pixel_put(&world->img, x, y, fcolor2hex(R_r));
			}
			else
			{
				my_mlx_pixel_put(&world->img, x, y, 0X6594EC);
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
	print_world(&world);
	mlx_loop_hook(world.mlx, &main_loop, &world);
	mlx_loop(world.mlx);
}
