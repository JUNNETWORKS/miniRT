#include "minirt.h"

t_object		*plane_init(t_vec3 center, t_vec3 normal, t_material material)
{
	t_object *plane;
	if (!(plane = malloc(sizeof(t_object))))
		return (NULL);
	plane->type = PLANE;
	plane->center = center;
	plane->normal = normal;
	plane->material = material;
	return (plane);
}

bool			plane_has_intersection(t_ray ray, t_object plane)
{
	double		d_n_dot;
	double		s_n_dot;
	double		p0_n_dot;
	double		t;

	d_n_dot = vec3_dot(vec3_mult(ray.direction, -1), plane.normal);
	// tの分母が0の場合, 交点は持たない
	if (d_n_dot == 0)
		return (false);
	t_vec3 center2camera = vec3_sub(ray.start, plane.center);
	t = vec3_dot(center2camera, plane.normal) / d_n_dot;
	return (t >= 0 ? true : false);
}

t_intersection	calc_plane_intersection(t_ray ray, t_object plane)
{
	double		d_n_dot;
	t_vec3		center2camera;
	double		t;
	t_intersection intersection;

	center2camera = vec3_sub(ray.start, plane.center);
	d_n_dot = vec3_dot(vec3_mult(ray.direction, -1), plane.normal);
	t = vec3_dot(center2camera, plane.normal) / d_n_dot;
	intersection.distance = t;
	// d_n_dot(cosθ)がマイナスだったらなす角が90°より大きいから法線ベクトルを逆向きになる
	intersection.normal = d_n_dot > 0 ? plane.normal : vec3_mult(plane.normal, -1);
	intersection.position = vec3_add(ray.start, vec3_mult(ray.direction, t));

	return (intersection);
}

