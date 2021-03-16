#include "minirt.h"

t_camera	*camera_init(t_vec3 pos, t_vec3 orientation, double fov)
{
	t_camera *camera;
	if (!(camera = malloc(sizeof(t_camera))))
		return (NULL);
	camera->pos = pos;
	camera->orientation = orientation;
	camera->fov = fov;
	return (camera);
}

