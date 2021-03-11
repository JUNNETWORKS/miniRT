#include "minirt.h"

t_object		*sphere_init(t_vec3 center, double radius)
{
	t_object *sphere;
	if (!(sphere = malloc(sizeof(t_object))))
		return (NULL);
	sphere->type = SPHERE;
	sphere->center = center;
	sphere->radius = radius;
	return (sphere);
}

bool			sphere_has_intersection(t_ray ray, t_object sphere)
{
	t_vec3 camera2sphere_vec = vec3_sub(ray.start, sphere.center);
	// レイが球に当たったか計算する
	double a = vec3_mag(ray.direction) * vec3_mag(ray.direction);
	double b = 2 * vec3_dot(camera2sphere_vec, ray.direction);
	double c = vec3_dot(camera2sphere_vec, camera2sphere_vec) - sphere.radius * sphere.radius;
	// 判別式
	double d = b * b - 4 * a * c;
	return (d >= 0);
}

t_intersection	calc_sphere_intersection(t_ray ray, t_object sphere)
{
	t_vec3 sphere2camera_vec = vec3_sub(ray.start, sphere.center);
	// レイが球に当たったか計算する
	double a = vec3_mag(ray.direction) * vec3_mag(ray.direction);
	double b = 2 * vec3_dot(sphere2camera_vec, ray.direction);
	double c = vec3_dot(sphere2camera_vec, sphere2camera_vec) - sphere.radius * sphere.radius;
	// 判別式
	double d = b * b - 4 * a * c;

	// レイと物体との交点の計算
	double t = (-b - sqrt(d)) / (2 * a);

	// レイと物体との交点
	t_vec3 p_i = vec3_add(ray.start, vec3_mult(ray.direction, t));

	// 物体面の法線ベクトルの計算
	t_vec3 n;
	n = vec3_normalize(vec3_sub(p_i, sphere.center));

	t_intersection intersection;
	intersection.distance = t;
	intersection.normal = n;
	intersection.position = p_i;
	return intersection;
}

