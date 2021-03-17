#include "minirt.h"

t_camera	*camera_init(t_world *world, t_vec3 pos, t_vec3 orientation, double fov)
{
	t_camera *camera;
	if (!(camera = malloc(sizeof(t_camera))))
		return (NULL);
	camera->pos = pos;
	camera->orientation = orientation;
	camera->fov = fov;
	// カメラからスクリーンまでの距離
	double d = world->screen_width / 2 / tan(deg2rad(camera->fov / 2));
	// カメラからスクリーンの中心へのベクトル
	camera->d_center = vec3_mult(camera->orientation, d);

	// スクリーンの基底ベクトル
	t_vec3 x_basis;  // x軸と並行なベクトル(スクリーンの世界)
	x_basis.x = camera->d_center.z / sqrt(camera->d_center.z * camera->d_center.z + camera->d_center.x * camera->d_center.x);
	x_basis.y = 0;
	x_basis.z = -camera->d_center.x / sqrt(camera->d_center.z * camera->d_center.z + camera->d_center.x * camera->d_center.x);
	t_vec3 y_basis;
	y_basis = vec3_normalize(vec3_cross(x_basis, vec3_mult(camera->d_center, -1)));
	
	camera->x_basis = x_basis;
	camera->y_basis = y_basis;
	return (camera);
}

