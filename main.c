#include "minirt.h"

int	raytracing(t_world *world, t_camera *camera)
{
	for (double x = 0; x < world->screen_width; x++){
		for (double y = 0; y < world->screen_height; y++){
			// スクリーン座標からワールド座標への変換
			// スクリーン上の位置
			double sw = x - (world->screen_width - 1) / 2;  // [-2/w ~ 2/w]
			double sh = (world->screen_height - 1) / 2 - y;  // [-2/h ~ 2/h]
			t_vec3 xx = vec3_mult(camera->x_basis, sw);
			t_vec3 yy = vec3_mult(camera->y_basis, sh);
			t_vec3 ray_direction;
			ray_direction = vec3_normalize(vec3_add(camera->d_center, vec3_add(xx, yy)));

			// レイ(光線)
			t_ray ray;
			ray.start = camera->pos;
			ray.direction = ray_direction;

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
				t_fcolor ref_diff_spec = fcolor_init(0, 0, 0);
				while (current_light)
				{
					// 対象の光源における拡散反射光
					t_fcolor ref_diff = calc_ref_diff(ray,
														*nearest_object_ptr,
														intersection,
														*(t_light*)current_light->content);
					// 対象の光源における鏡面反射光
					t_fcolor ref_spec = calc_ref_spec(ray,
														*nearest_object_ptr,
														intersection,
														*(t_light*)current_light->content);
					// 物体表面の反射輝度(拡散反射光+鏡面反射光)の計算
					t_fcolor current_ref_diff_spec = fcolor_add(ref_diff, ref_spec);
					if (has_shadow(world, *(t_light*)current_light->content, intersection))
						current_ref_diff_spec = fcolor_init(0,0,0);
					ref_diff_spec = fcolor_add(ref_diff_spec, current_ref_diff_spec);
					current_light = current_light->next;
				}
				// 環境光R_a
				t_fcolor ref_ambient = fcolor_mult(nearest_object_ptr->material.kDif, world->ambient);
				// t_fcolor ref_ambient = fcolor_init(0, 0, 0);
				// 最終的な輝度  環境光 + (拡散反射光(Diffuse) + 鏡面反射光(Specular))
				t_fcolor ref_result = fcolor_add(ref_ambient, ref_diff_spec);

				my_mlx_pixel_put(&camera->img, x, y, fcolor2hex(ref_result));
			}
			else
			{
				// my_mlx_pixel_put(&camera->img, x, y, 0X6594EC);
				my_mlx_pixel_put(&camera->img, x, y, 0);
			}
		}
	}
	return (0);
}

int	render_all_cameras(t_world *world)
{
	t_dlist *first_camera;  // ループ用
	t_camera *camera;

	first_camera = world->cameras;
	do {
		camera = (t_camera*)world->cameras->content;
		raytracing(world, camera);
		printf("DONE rendering camera at %p\n", camera);
		world->cameras = world->cameras->next;
	} while(world->cameras != first_camera);
	printf("DONE all camera rendering\n");
	return (0);
}

int	main_loop(t_world *world)
{
	t_camera	*camera;

	camera = (t_camera*)world->cameras->content;
	mlx_put_image_to_window(world->mlx, world->win, camera->img.img, 0, 0);
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
		render_all_cameras(&world);
		write_world2bmp(&world);
		exit(EXIT_SUCCESS);
	}
	if (configure_screen(&world, true))
		return (EXIT_FAILURE);
	// initialize_objects(&world);
	print_world(&world);
	render_all_cameras(&world);
	mlx_hook(world.win, KeyPress, KeyPressMask, key_press_hook, &world);
	mlx_hook(world.win, ClientMessage, 1L << 17, exit_world, &world);
	mlx_loop_hook(world.mlx, &main_loop, &world);
	mlx_loop(world.mlx);
}
