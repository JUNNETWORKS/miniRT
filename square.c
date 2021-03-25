#include "minirt.h"

t_object		*square_init(t_vec3 center, t_vec3 normal, double side_size, t_material material)
{
	t_object *square;
	if (!(square = malloc(sizeof(t_object))))
		return (NULL);
	square->type = SQUARE;
	square->center = center;
	square->side_size = side_size;
	square->material = material;
	return (square);
}

t_intersection	calc_square_intersection(t_ray ray, t_object square)
{

}

