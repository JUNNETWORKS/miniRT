#include "minirt.h"

t_object *sphere_init(t_vec3 center, double radius)
{
	t_object *sphere;
	if (!(sphere = malloc(sizeof(t_object))))
		return (NULL);
	sphere->type = SPHERE;
	sphere->center = center;
	return (sphere);
}

