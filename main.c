#include "minirt.h"

int		initialize_objects(t_world *world)
{
	t_object *object;

	if (!(object = sphere_init(vec3_init(3, 0, 25), 1,
			material_init(fcolor_init(0.01, 0.01, 0.01),
							fcolor_init(0.69, 0.0, 0.0),
							fcolor_init(0.3, 0.3, 0.3),
							8.0))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(2, 0, 20), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.0, 0.69, 0.0),
								fcolor_init(0.3, 0.3, 0.3),
								8.0))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(1, 0, 15), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.0, 0.0, 0.69),
								fcolor_init(0.3, 0.3, 0.3),
								8.0))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(0, 0, 10), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.0, 0.69, 0.69),
								fcolor_init(0.3, 0.3, 0.3),
								8.0))) ||
			!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = sphere_init(vec3_init(-1, 0, 5), 1,
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.69, 0.0, 0.69),
								fcolor_init(0.3, 0.3, 0.3),
								8.0))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	if (!(object = plane_init(vec3_init(0, -1, 0), vec3_init(0, 1, 0), 
				material_init(fcolor_init(0.01, 0.01, 0.01),
								fcolor_init(0.69, 0.69, 0.69),
								fcolor_init(0.3, 0.3, 0.3),
								8.0))) ||
		!(ft_lstadd_back_new(&world->objects, object)))
		return (put_and_return_err("failed malloc object"));
	// 光源
	t_light *light;
	if (!(light = light_init(vec3_init(-5, 5, -5), fcolor_init(0.5, 0.5, 0.5))) ||
		!(ft_lstadd_back_new(&world->lights, light)))
		return (put_and_return_err("failed malloc light"));
	if (!(light = light_init(vec3_init(5, 0, -5), fcolor_init(0.5, 0.5, 0.5))) ||
		!(ft_lstadd_back_new(&world->lights, light)))
		return (put_and_return_err("failed malloc light"));
	if (!(light = light_init(vec3_init(5, 20, -5), fcolor_init(0.5, 0.5, 0.5))) ||
		!(ft_lstadd_back_new(&world->lights, light)))
		return (put_and_return_err("failed malloc light"));
	// 環境光
	world->ambient_intensity = fcolor_init(0.1, 0.1, 0.1);
	// カメラ
	t_camera *camera;
	if (!(camera = camera_init(vec3_init(0, 0, -5), vec3_init(0, 0, 1), 66)) ||
		!(dlst_add_right_new(&world->cameras, (void*)camera)))
		return (put_and_return_err("failed malloc camera"));
	return (0);
}

int	raytracing(t_world *world)
{
	// 視点位置を表すベクトル
	t_vec3 camera_vec;
	camera_vec = vec3_init(0, 0, -5);  // スクリーンの少し手前な感じ

	t_camera camera;
	camera = *(t_camera*)world->cameras->content;

	// カメラにおける正規直交ベクトルXYZ
	t_vec3 X, Y, Z;
	Z = vec3_normalize(camera.orientation);
	X = vec3_normalize(vec3_cross(vec3_init(0, 1, 0), Z));
	Y = vec3_normalize(vec3_cross(X, Z));
	// スクリーンの基底ベクトルu,v,w
	double half_h = tan(deg2rad(camera.fov) / 2.0);
	double aspect = world->screen_width / world->screen_height;
	double half_w = half_h * aspect;
	t_vec3 u_vec, v_vec, w_vec;
	u_vec = vec3_mult(X, 2 * half_w);
	v_vec = vec3_mult(Y, 2 * half_h);
	t_vec3 wX = vec3_mult(X, half_w);
	t_vec3 hY = vec3_mult(Y, half_h);
	// w = o - wX - hY - Z
	w_vec = vec3_add(vec3_sub(vec3_sub(camera.pos, wX), hY), Z);
	printf("Z: ");print_vec3(Z);printf("\n");
	printf("w_vec: ");print_vec3(w_vec);printf("\n");

	// 点光源(light)
	t_vec3 light_vec;
	light_vec = vec3_init(-5, 5, -5);
	for (double x = 0; x < world->screen_width; x++){
		for (double y = 0; y < world->screen_height; y++){
			// スクリーン座標からワールド座標への変換
			// x,yは[0,1]へ変換する
			// スクリーン上の位置
			t_vec3 screen_vec;
			double u = x / (world->screen_width - 1);
			double v = y / (world->screen_height - 1);
			screen_vec = vec3_add(vec3_add(vec3_mult(u_vec, u), vec3_mult(v_vec, v)), w_vec);
			// printf("screen: ");print_vec3(screen_vec);printf("\n");
			// screen_vec = vec3_add(vec3_mult(u_vec, u), vec3_mult(v_vec, v));

			// レイ(光線)
			t_ray ray;
			ray.start = camera_vec;
			ray.direction = vec3_normalize(vec3_sub(screen_vec, camera.pos));

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
					if (has_shadow(world, *(t_light*)current_light->content, intersection))
						current_R_ds = fcolor_init(0,0,0);
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

int main(int argc, char **argv)
{
	t_world world;

	if (argc < 2 || argc > 3)
		put_and_exit_err("args count is incorrect!");
	if ((initialize_world(&world)) == ERROR ||
		(load_rtfile(&world, argv[1])) == ERROR)
		put_and_exit_err("Error is occured when load .rt file");
	if (argc == 3)
	{
		if (ft_strncmp(argv[2], "--save", ft_strlen("--save") + 1))
			put_and_exit_err("argv is not \"--save\"");
		if (configure_screen(&world, false))
			return (EXIT_FAILURE);
		// TODO: write bmp
		exit(EXIT_SUCCESS);
	}
	if (configure_screen(&world, true))
		return (EXIT_FAILURE);
	// initialize_objects(&world);
	print_world(&world);
	mlx_hook(world.win, KeyPress, KeyPressMask, key_press_hook, &world);
	mlx_hook(world.win, ClientMessage, 1L << 17, exit_world, &world);
	mlx_loop_hook(world.mlx, &main_loop, &world);
	mlx_loop(world.mlx);
}
