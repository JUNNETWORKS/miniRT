#include "minirt.h"

// 負の数は0に, 1.0より上は1.0にする
t_fcolor	fcolor_normalize(t_fcolor fcolor)
{
	// 負の数は0にする
	fcolor.red = fcolor.red < 0 ? 0.0 : fcolor.red;
	fcolor.green = fcolor.green < 0 ? 0.0 : fcolor.green;
	fcolor.blue = fcolor.blue < 0 ? 0.0 : fcolor.blue;
	// 1.0より上は1.0にする
	fcolor.red = fcolor.red > 1.0 ? 1.0 : fcolor.red;
	fcolor.green = fcolor.green > 1.0 ? 1.0 : fcolor.green;
	fcolor.blue = fcolor.blue > 1.0 ? 1.0 : fcolor.blue;
	return (fcolor);
}

t_fcolor	fcolor_init(double red, double green, double blue)
{
	t_fcolor new;
	new.red = red;
	new.green = green;
	new.blue = blue;
	return (fcolor_normalize(new));
}

// fcolorの足し算. 各チャンネルごとに足す
t_fcolor	fcolor_add(t_fcolor a, t_fcolor b)
{
	a.red += b.red;
	a.green += b.green;
	a.blue += b.blue;
	return (fcolor_normalize(a));
}

// fcolorの掛け算. 各チャンネルごとに掛ける
t_fcolor	fcolor_mult(t_fcolor a, t_fcolor b)
{
	a.red *= b.red;
	a.green *= b.green;
	a.blue *= b.blue;
	return (fcolor_normalize(a));
}

// fcolorとスカラーの掛け算. 各チャンネルごとに掛ける
t_fcolor	fcolor_mult_scalar(t_fcolor a, double b)
{
	a.red *= b;
	a.green *= b;
	a.blue *= b;
	return (fcolor_normalize(a));
}

// "r,g,b"をパースして*fcolorにt_fcolorを代入する
int			get_fcolor_from_rgbstr(t_fcolor *fcolor, char *rgbstr)
{
	uint32_t	rgbhex;
	if (get_rgbhex_from_rgbstr(&rgbhex, rgbstr) == ERROR)
		return	(put_and_return_err("failed parse rgbstr"));
	double red = (rgbhex >> 16 & 0xff) / 255;
	double green = (rgbhex >> 8 & 0xff) / 255;
	double blue = (rgbhex & 0xff) / 255;
	*fcolor = fcolor_normalize(fcolor_init(red, green, blue));
	return (0);
}
