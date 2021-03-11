#include "minirt.h"

t_object *plane_init(t_vec3 center, t_vec3 normal)
{
	t_object *plane;
	if (!(plane = malloc(sizeof(t_object))))
		return (NULL);
	plane->type = PLANE;
	plane->center = center;
	plane->normal = normal;
	return (plane);
}

