#include "minirt.h"

int	initialize_world(t_world *world)
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

int	raytracing(t_world *world)
{
	// 視点位置を表すベクトル
	t_vec3 camera_vec;
	camera_vec = vec3_init(0, 0, -5);  // スクリーンの少し手前な感じ

	// 球の中心座標
	t_vec3 sphere_vec;
	sphere_vec = vec3_init(0, 0, 5);  // スクリーンの少し奥な感じ
	double sphere_r = 1;  // 半径

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

			// 方向ベクトル
			t_vec3 dir_vec;
			dir_vec = vec3_normalize(vec3_sub(screen_vec, camera_vec));

			t_vec3 camera2sphere_vec = vec3_sub(camera_vec, sphere_vec);

			// レイが球に当たったか計算する
			double a = vec3_mag(dir_vec) * vec3_mag(dir_vec);
			double b = 2 * vec3_dot(camera2sphere_vec, dir_vec);
			double c = vec3_dot(camera2sphere_vec, camera2sphere_vec) - sphere_r * sphere_r;
			// 判別式
			double d = b * b - 4 * a * c;
			if (d >= 0)
			{
				// レイと物体との交点の計算
				double t = (-b - sqrt(d)) / (2 * a);
				if (d > 0){  // d > 0 の時, 2つの交点を持つ
					double t1 = (-b + sqrt(d)) / (2 * a);
					// 近い方の交点を描画する
					t = t < t1 ? t : t1;
				}
				// レイと物体との交点
				t_vec3 p_i = vec3_add(camera_vec, vec3_mult(dir_vec, t));
				if (t < 0)
					continue;

				// 光の入射ベクトルの計算
				t_vec3 l;
				l = vec3_normalize(vec3_sub(light_vec, p_i));

				// 物体面の法線ベクトルの計算
				t_vec3 n;
				n = vec3_normalize(vec3_sub(p_i, sphere_vec));

				double ray_deg = vec3_dot(n, l);
				// 法線ベクトルと入射ベクトルの成す角がπ/2を超えた時, 光は裏側から当たっているので反射は起きない
				if (ray_deg < 0)
					ray_deg = 0;

				// 拡散反射光R_dの計算
				// 光源の強度I_i
				double I_i = 1.0;
				// 拡散反射係数k_d
				double k_d = 0.69;
				// 拡散反射光の放射輝度R_d
				double R_d = k_d * I_i * ray_deg;

				// 環境光R_aの計算
				// 環境光の強度
				double I_a = 0.1;
				// 環境光反射係数
				double k_a = 0.01;
				// 環境光R_a
				double R_a = k_a * I_a;

				// 鏡面反射光R_sの計算
				// 鏡面反射係数k_s
				double k_s = 0.3;
				// 光沢度α
				double alpha = 8;
				// 視線ベクトルの逆ベクトル
				t_vec3 v = vec3_mult(dir_vec, -1);
				// 入射光の正反射ベクトル
				t_vec3 r = vec3_sub(vec3_mult(vec3_mult(n, vec3_dot(n, l)), 2), l);
				// 鏡面反射光の放射強度R_s
				double R_s = k_s * I_i * pow(vec3_dot(v, r), alpha);
				if (vec3_dot(v, r) < 0)
					R_s = 0;

				// 最終的な輝度  (環境光 + 拡散反射光 + 鏡面反射光)
				double R_r = R_a + R_d + R_s;
				my_mlx_pixel_put(&world->img, x, world->screen_height - y - 1, rgb2hex((int)(255 * R_r), 0, 0));
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
	mlx_loop_hook(world.mlx, &main_loop, &world);
	mlx_loop(world.mlx);
}
