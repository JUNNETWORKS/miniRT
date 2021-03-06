#include "minirt.h"

// mlx_pixel_put() はレンダリングを待たずに1ピクセルずつ描画するので激遅, なので同じ動きをする関数を自作する
void		my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char	*dst;

	// line_lengthは実際のウィンドウの横幅と違うので計算する必要がある
    dst = img->addr + (y * img->line_length +
		x * (img->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

uint32_t	get_color_from_img(t_img img, int x, int y)
{
	return *(uint32_t*)(img.addr +
		(y * img.line_length + x * (img.bits_per_pixel / 8)));
}

