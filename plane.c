#include "minirt.h"

t_object		*plane_init(t_vec3 center, t_vec3 normal)
{
	t_object *plane;
	if (!(plane = malloc(sizeof(t_object))))
		return (NULL);
	plane->type = PLANE;
	plane->center = center;
	plane->normal = normal;
	return (plane);
}

bool			plane_has_intersection(t_ray ray, t_object plane)
{
	double		d_n_dot;
	double		s_n_dot;
	double		p0_n_dot;
	double		t;

	d_n_dot = vec3_dot(ray.direction, plane.normal);
	// tの分母が0の場合, 交点は持たない
	if (d_n_dot == 0)
		return (false);
	s_n_dot = vec3_dot(ray.start, plane.normal);
	p0_n_dot = vec3_dot(plane.center, plane.normal);
	t = (s_n_dot - p0_n_dot) / d_n_dot;
	return (t >= 0 ? true : false);
}

t_intersection	calc_plane_intersection(t_ray ray, t_object plane)
{
	double		d_n_dot;
	double		s_n_dot;
	double		p0_n_dot;
	double		t;
	t_intersection intersection;

	d_n_dot = vec3_dot(ray.direction, plane.normal);
	s_n_dot = vec3_dot(ray.start, plane.normal);
	p0_n_dot = vec3_dot(plane.center, plane.normal);
	t = (s_n_dot - p0_n_dot) / d_n_dot;
	intersection.distance = t;
	intersection.normal = plane.normal;
	intersection.position = vec3_add(ray.start, vec3_mult(ray.direction, t));

	return (intersection);
}

