#include "minirt.h"

t_object		*cylinder_init(t_vec3 point, t_vec3 normal, double radius, double height, t_material material)
{
	t_object *cylinder;
	if (!(cylinder = malloc(sizeof(t_object))))
		return (NULL);
	cylinder->type = CYLINDER;
	cylinder->center = point;
	cylinder->normal = normal;
	cylinder->radius = radius;
	cylinder->height = height;
	cylinder->material = material;
	return (cylinder);
}

t_intersection	calc_cylinder_intersection(t_ray ray, t_object cylinder)
{
	t_intersection intersection;
	t_vec3 sphere2camera_vec = vec3_sub(ray.start, cylinder.center);
	// レイが球に当たったか計算する
	double a = vec3_mag(vec3_cross(ray.direction, cylinder.normal));
	a = a * a;
	double b = 2 * vec3_dot(vec3_cross(ray.direction, cylinder.normal), vec3_cross(vec3_sub(ray.start, cylinder.center), cylinder.normal));
	double c = vec3_mag(vec3_cross(vec3_sub(ray.start, cylinder.center), cylinder.normal));
	c = c * c - cylinder.radius * cylinder.radius;
	// 判別式
	double d = b * b - 4 * a * c;

	if (d < 0)
	{
		intersection.has_intersection = false;
		return (intersection);
	}

	// レイと円筒との距離を求める
	double t_outer = (-b - sqrt(d)) / (2 * a);  // 円筒の外側
	double t_inner = (-b + sqrt(d)) / (2 * a);  // 円筒の内側

	// レイと縦に無限に伸びる円筒との交点
	t_vec3 p_outer = vec3_add(ray.start, vec3_mult(ray.direction, t_outer));
	t_vec3 p_inner = vec3_add(ray.start, vec3_mult(ray.direction, t_inner));

	t_vec3 center2p_outer = vec3_sub(p_outer, cylinder.center);
	t_vec3 center2p_inner = vec3_sub(p_inner, cylinder.center);

	// 底面から交点までの高さ
	double height_outer = vec3_dot(center2p_outer, cylinder.normal);
	double height_inner = vec3_dot(center2p_inner, cylinder.normal);

	if (height_outer >= 0 && height_outer <= cylinder.height)
	{
		intersection.has_intersection = true;
		intersection.normal = vec3_normalize(vec3_sub(center2p_outer, vec3_mult(cylinder.normal, height_outer)));
		intersection.distance = t_outer;
		intersection.position = p_outer;
	}
	else if (height_inner >= 0 && height_inner <= cylinder.height)
	{
		intersection.has_intersection = true;
		intersection.normal = vec3_normalize(vec3_sub(vec3_mult(cylinder.normal, height_inner), center2p_inner));
		intersection.distance = t_inner;
		intersection.position = p_inner;
	}
	else
	{
		intersection.has_intersection = false;
	}
	return intersection;
}

