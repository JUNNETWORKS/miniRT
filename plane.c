#include "minirt.h"

t_plane *plane_init(t_vec3 center, t_vec3 normal)
{
	t_plane *plane;
	if (!(plane = malloc(sizeof(plane))))
		return (NULL);
	plane->type = PLANE;
	plane->center = center;
	plane->normal = normal;
	return (plane);
}

