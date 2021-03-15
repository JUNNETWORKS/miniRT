#include "minirt.h"

// アルファブレンディング
// dst: 背景, src: 前景
uint32_t	alpha_blend(uint32_t dst, uint32_t src)
{
	double		alpha;
	uint32_t	color;

	// 透明: 0.0, 不透明: 1.0
	alpha = -((double)(src >> 24) / (double)0xff - 1.0);
	color = 0x00000000;
	color |= (int)((src & 0xff) * alpha) + (int)((dst & 0x000000ff) * (1 - alpha));
	color |= ((int)((src >> 8 & 0xff) * alpha) + (int)((dst >> 8 & 0xff) * (1 - alpha))) << 8;
	color |= ((int)((src >> 16 & 0xff) * alpha) + (int)((dst >> 16 & 0xff) * (1 - alpha))) << 16;
	return (color);
}

uint32_t	rgb2hex(int r, int g, int b)
{
	uint32_t	color;

	color = 0;
	color |= b;
	color |= g << 8;
	color |= r << 16;
	return (color);
}

bool		is_valid_color(int r, int g, int b)
{
	printf("rgb: (%d, %d, %d)\n", r, g, b);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (false);
	return (true);
}

uint32_t	get_rgbhex_from_rgbstr(uint32_t *color, char *rgbstr)
{
	char	**rgb;

	if (str_c_count(rgbstr, ',') != 2 || get_split_size(rgbstr, ',') != 3)
		return (put_and_return_err("rgb is wrong"));
	if (!(rgb = ft_split(rgbstr, ',')))
		return (ERROR);
	if (!str_all_true(rgb[0], ft_isdigit) ||
			!str_all_true(rgb[1], ft_isdigit) ||
			!str_all_true(rgb[2], ft_isdigit) ||
			!is_valid_color(ft_atoi(rgb[0]), ft_atoi(rgb[1]), ft_atoi(rgb[2])) ||
			(rgb[0][0] == '0' && rgb[0][1]) ||
			(rgb[1][0] == '0' && rgb[1][1]) ||
			(rgb[2][0] == '0' && rgb[2][1])){
		free_ptrarr((void**)rgb);
		return (ERROR);
	}
	*color = ft_atoi(rgb[0]) << 16 | ft_atoi(rgb[1]) << 8 | ft_atoi(rgb[2]);
	free_ptrarr((void**)rgb);
	return (0);
}

// 放射輝度から32bit色に変換する
uint32_t	fcolor2hex(t_fcolor fcolor)
{
	return (rgb2hex(fcolor.red * 255, fcolor.green * 255, fcolor.blue * 255));
};
