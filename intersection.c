#include "minirt.h"

t_intersection	calc_intersection(t_ray ray, t_object object)
{
	if (object.type == PLANE)
		return (calc_plane_intersection(ray, object));
	else if (object.type == SPHERE)
		return (calc_sphere_intersection(ray, object));
	else if (object.type == TRIANGLE)
		return (calc_triangle_intersection(ray, object));
	else if (object.type == SQUARE)
		return (calc_square_intersection(ray, object));
}

// レイがどのオブジェクトとも交点を持たない時にNULLを返す
t_object		*get_nearest_object(t_world *world, t_ray ray)
{
	t_object			*nearest_object;
	t_object			*current_object;
	double				nearest_distance;
	t_list				*current_lst;
	t_intersection		intersection;

	nearest_distance = INFINITY;
	nearest_object = NULL;
	current_lst = world->objects;
	while (current_lst)
	{
		current_object = (t_object*)current_lst->content;
		intersection = calc_intersection(ray, *current_object);
		if (intersection.has_intersection)
		{
			if (intersection.distance >= 0 && intersection.distance <= nearest_distance)
			{
				nearest_object = current_object;
				nearest_distance = intersection.distance;
			}
		}
		current_lst = current_lst->next;
	}
	return (nearest_object);
}
