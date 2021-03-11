#include "minirt.h"

bool			has_intersection(t_ray ray, t_object object)
{
	if (object.type == PLANE)
		return (plane_has_intersection(ray, object));
	else if (object.type == SPHERE)
		return (sphere_has_intersection(ray, object));
}

t_intersection	calc_intersection(t_ray ray, t_object object)
{
	if (object.type == PLANE)
		return (calc_plane_intersection(ray, object));
	else if (object.type == SPHERE)
		return (calc_sphere_intersection(ray, object));
}

// レイがどのオブジェクトとも交点を持たない時にNULLを返す
t_object		*get_nearest_object(t_world *world, t_ray ray)
{
	t_object	*nearest_object;
	t_object	*current_object;
	double		nearest_distance;
	double		current_distance;
	t_list		*current_lst;

	nearest_distance = DBL_MAX;
	nearest_object = NULL;
	current_lst = world->objects;
	while (current_lst)
	{
		current_object = (t_object*)current_lst->content;
		if (has_intersection(ray, *current_object))
		{
			current_distance = (calc_intersection(ray, *current_object)).distance;
			if (current_distance >= 0 && current_distance <= nearest_distance)
			{
				nearest_object = current_object;
				nearest_distance = current_distance;
			}
		}
		current_lst = current_lst->next;
	}
	return (nearest_object);
}
