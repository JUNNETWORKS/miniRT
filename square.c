#include "minirt.h"

t_object		*square_init(t_vec3 center, t_vec3 normal, double side_size, t_material material)
{
	t_object *square;
	if (!(square = malloc(sizeof(t_object))))
		return (NULL);
	square->type = SQUARE;
	square->center = center;
	square->normal = normal;
	square->side_size = side_size;
	square->material = material;
	return (square);
}

static bool		is_point_in_square(t_vec3 point, t_object square)
{
	t_vec3		dist;  // 四角形の中心からpointまでのベクトル

	dist = vec3_sub(point, square.center);
	if (dist.x >= -square.side_size / 2 && dist.x <= square.side_size / 2 &&
		dist.y >= -square.side_size / 2 && dist.y <= square.side_size / 2)
		return (true);
	return (false);
}

/*
 * 四角形の交点の求め方
 *   1. 平面と同じ方法で交点を求める
 *   2. その交点が四角形の範囲内に入っているか計算する
 */
t_intersection	calc_square_intersection(t_ray ray, t_object square)
{
	double		d_n_dot;
	t_vec3		center2camera;
	double		t;
	t_intersection intersection;

	center2camera = vec3_sub(ray.start, square.center);
	d_n_dot = vec3_dot(vec3_mult(ray.direction, -1), square.normal);
	if (d_n_dot == 0)
	{
		intersection.has_intersection = false;
		return (intersection);
	}
	t = vec3_dot(center2camera, square.normal) / d_n_dot;
	intersection.has_intersection = true;
	intersection.distance = t;
	// d_n_dot(cosθ)がマイナスだったらなす角が90°より大きいから法線ベクトルを逆向きになる
	intersection.normal = d_n_dot > 0 ? square.normal : vec3_mult(square.normal, -1);
	intersection.position = vec3_add(ray.start, vec3_mult(ray.direction, t));
	intersection.has_intersection = is_point_in_square(intersection.position, square);
	return (intersection);
}

