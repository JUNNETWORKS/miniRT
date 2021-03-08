#include "minirt.h"

void	clear_img(t_img *img)
{
	int	x;
	int	y;

	x = 0;
	while (x < img->width)
	{
		y = 0;
		while (y < img->height)
		{
			my_mlx_pixel_put(img, x, y, 0xff000000);
			y++;
		}
		x++;
	}
}

