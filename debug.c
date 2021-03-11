#include "minirt.h"

char	*get_type_name(enum e_shape type)
{
	if (type == PLANE)
		return ("PLANE");
	else if (type == SPHERE)
		return ("SPHERE");
	else if (type == SQUARE)
		return ("SQUARE");
	else if (type == CYLINDER)
		return ("CYLINDER");
	else if (type == TRIANGLE)
		return ("TRIANGLE");
	return ("INVALID");
}

void	print_world(t_world *world)
{
	printf("world:\t%p\n", world);
	printf("\tscreen_width:  %d\n", world->screen_width);
	printf("\tscreen_height: %d\n", world->screen_height);
	printf("\tobjects:       %p\n", world->objects);
	t_list *current_lst = world->objects;
	while (current_lst){
		printf("\t\ttype: %s\n", get_type_name(((t_object*)current_lst->content)->type));
		current_lst = current_lst->next;
	}
}

void	print_vec3(t_vec3 vec)
{
	printf("{x: %lf,y: %lf, z: %lf}", vec.x, vec.y, vec.z);
}

void	print_object(t_object object)
{
	printf("type: %d (%s)\n", object.type, get_type_name(object.type));
	printf("center: ");
	print_vec3(object.center);
	printf("\n");
	printf("normal: ");
	print_vec3(object.normal);
	printf("\n");
	printf("radius: %lf\n", object.radius);
}
