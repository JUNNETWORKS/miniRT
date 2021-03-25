#include "minirt.h"

t_object		*triangle_init(t_vec3 p1, t_vec3 p2, t_vec3 p3, t_material material)
{
	t_object *triangle;
	if (!(triangle = malloc(sizeof(t_object))))
		return (NULL);
	triangle->type = TRIANGLE;
	triangle->p1 = p1;
	triangle->p2 = p2;
	triangle->p3 = p3;
	triangle->material = material;
	return (triangle);
}

// 三角形の内外判定
static bool		is_point_in_triangle(t_vec3 point, t_object triangle)
{
	t_vec3	cross_1;
	t_vec3	cross_2;
	t_vec3	cross_3;

	cross_1 = vec3_normalize(vec3_cross(vec3_sub(triangle.p3, triangle.p1), vec3_sub(point, triangle.p1)));
	cross_2 = vec3_normalize(vec3_cross(vec3_sub(triangle.p1, triangle.p2), vec3_sub(point, triangle.p2)));
	cross_3 = vec3_normalize(vec3_cross(vec3_sub(triangle.p2, triangle.p3), vec3_sub(point, triangle.p3)));

	// 外積がそれぞれ同じ方向を向いているか
	bool	check1, check2, check3;
	check1 = vec3_dot(cross_1, cross_2) < 0;
	check2 = vec3_dot(cross_2, cross_3) < 0;
	check3 = vec3_dot(cross_3, cross_1) < 0;
	if ((check1 && check2 && check3) || (!check1 && !check2 && !check3))
		return (true);
	return (false);
}

/*
 * 三角形の交点の求め方
 *   1. 適当に法線を求める.
 *   2. 平面と同じ方法で交点を求める.(一旦三角形を平面として考える)
 *   3. 三角形の内外判定を使ってその交点が三角形に入っているか判定する
 */
t_intersection	calc_triangle_intersection(t_ray ray, t_object triangle)
{
	t_vec3			triangle_normal;  // 三角形平面の法線ベクトル
	double			d_n_dot;
	t_vec3			center2camera;
	double			t;
	t_intersection	intersection;

	triangle_normal = vec3_cross(vec3_sub(triangle.p3, triangle.p1), vec3_sub(triangle.p2, triangle.p1));
	triangle_normal = vec3_normalize(triangle_normal);
	center2camera = vec3_sub(ray.start, triangle.p1);
	d_n_dot = vec3_dot(vec3_mult(ray.direction, -1), triangle_normal);
	if (d_n_dot == 0)
	{
		intersection.has_intersection = false;
		return (intersection);
	}
	t = vec3_dot(center2camera, triangle_normal) / d_n_dot;
	intersection.distance = t;
	// d_n_dot(cosθ)がマイナスだったらなす角が90°より大きいから法線ベクトルを逆向きになる
	intersection.normal = d_n_dot > 0 ? triangle_normal : vec3_mult(triangle_normal, -1);
	intersection.position = vec3_add(ray.start, vec3_mult(ray.direction, t));
	intersection.has_intersection = is_point_in_triangle(intersection.position, triangle);
	return (intersection);
}

