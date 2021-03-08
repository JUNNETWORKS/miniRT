#ifndef MINIRT_H
# define MINIRT_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include <X11/X.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <errno.h>
# include <string.h>

// 描画するためのイメージ情報を保持
typedef struct	s_img {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_img;

typedef struct	s_world {
	void		*mlx;
	void		*win;
	t_img		img;
	int			screen_width;
	int			screen_height;
}				t_world;

// Vector3D
typedef struct	s_vec3 {
	double		x;
	double		y;
	double		z;
} t_vec3;

// MLX Utils
void			my_mlx_pixel_put(t_img *img, int x, int y, int color);
uint32_t		get_color_from_img(t_img img, int x, int y);
void			clear_img(t_img *img);
// Vec3 Utils
t_vec3			vec3_init(double x, double y, double z);
t_vec3			vec3_add(t_vec3 a, t_vec3 b);
t_vec3			vec3_sub(t_vec3 a, t_vec3 b);
t_vec3			vec3_mult(t_vec3 a, double b);
double			vec3_dot(t_vec3 a, t_vec3 b);
t_vec3			vec3_cross(t_vec3 a, t_vec3 b);
double			vec3_mag(t_vec3 a);
t_vec3			vec3_normalize(t_vec3 a);
// Utils
double			deg2rad(int x);
int				rad2deg(double x);
uint32_t		rgb2hex(int r, int g, int b);
uint32_t		alpha_blend(uint32_t dst, uint32_t src);
void			put_error_msg(char *str);
int				put_and_return_err(char *str);
bool			str_all_true(char *str, int(*f)(int));
size_t			str_c_count(char *str, char c);
size_t			ptrarr_len(void **ptrarr);
void			free_ptrarr(void **ptrarr);
void			free_ptrarr_and_assign_null(void ***ptrarr);
void			free_and_assign_null(void **p);

#endif
