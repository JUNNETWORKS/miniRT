#include "minirt.h"

t_sphere *sphere_init(t_vec3 center, double radius)
{
	t_sphere *sphere;
	if (!(sphere = malloc(sizeof(t_sphere))))
		return (NULL);
	sphere->type = SPHERE;
	sphere->center = center;
	return (sphere);
}

