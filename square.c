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

/* 四角形の内外判定のやり方
 *   1. カメラと同じように四角形平面空間上での基底ベクトルを求める.
 *      つまり, 四角形平面をスクリーンのような感じでx,yの2軸で表せるようにする.
 *   2. 求めた四角形平面空間上での基底ベクトルと四角形の中心から平面上の交点とのベクトルで内積を取ってx,yの距離を求める.
 *      求めたx,yの距離を使って内外判定を行う.
 */
static bool		is_point_in_square(t_vec3 point, t_object square)
{
	// 四角形平面空間上での基底ベクトル
	t_vec3		x_basis;
	t_vec3		y_basis;
	x_basis.x = square.normal.z / sqrt(square.normal.z * square.normal.z + square.normal.x * square.normal.x);
	x_basis.y = 0;
	x_basis.z = -square.normal.x / sqrt(square.normal.z * square.normal.z + square.normal.x * square.normal.x);
	x_basis = vec3_normalize(x_basis);
	y_basis = vec3_normalize(vec3_cross(x_basis, vec3_mult(square.normal, -1)));

	t_vec3		o_p;  // 四角形の中心からpointまでのベクトル
	o_p = vec3_sub(point, square.center);
	// x, yにおける距離を求める
	double		x_dist;
	double		y_dist;
	x_dist = vec3_dot(o_p, x_basis);
	y_dist = vec3_dot(o_p, y_basis);
	if (x_dist >= -square.side_size / 2 && x_dist <= square.side_size / 2 &&
		y_dist >= -square.side_size / 2 && y_dist <= square.side_size / 2)
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
	intersection.distance = t;
	// d_n_dot(cosθ)がマイナスだったらなす角が90°より大きいから法線ベクトルを逆向きになる
	intersection.normal = d_n_dot > 0 ? square.normal : vec3_mult(square.normal, -1);
	intersection.position = vec3_add(ray.start, vec3_mult(ray.direction, t));
	intersection.has_intersection = is_point_in_square(intersection.position, square);
	return (intersection);
}

