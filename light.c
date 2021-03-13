#include "minirt.h"

t_light		*light_init(t_vec3 position, t_fcolor intensity)
{
	t_light *light;
	if (!(light = malloc(sizeof(t_light))))
		return (NULL);
	light->position = position;
	light->intensity = intensity;
	return (light);
}

