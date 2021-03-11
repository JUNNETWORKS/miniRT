#include "minirt.h"

void	print_world(t_world *world)
{
	printf("world:\t%p\n", world);
	printf("\tscreen_width:  %d\n", world->screen_width);
	printf("\tscreen_height: %d\n", world->screen_height);
	printf("\tobjects:       %p\n", world->objects);
	t_list *current_lst = world->objects;
	while (current_lst){
		printf("\t\ttype: %u\n", ((t_object*)current_lst->content)->type);
		current_lst = current_lst->next;
	}
}

