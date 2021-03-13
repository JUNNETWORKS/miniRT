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
# include <float.h>

// Vector3D
typedef struct	s_vec3 {
	double		x;
	double		y;
	double		z;
} t_vec3;

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

// レイ(光線)を表す構造体
typedef struct	s_ray {
	// レイの始点
	t_vec3		start;
	// レイの方向ベクトル
	t_vec3		direction;
}				t_ray;

// 反射係数をRGBごとに保持する構造体
typedef struct	s_fcolor {
	double		red;
	double		green;
	double		blue;
}				t_fcolor;

// ライティングに関する情報を持つ構造体
typedef struct	s_lighting {
	// 光源までの距離
	t_vec3		distance;
	// ライティングの強度
	t_fcolor	intensity;
	// ライティングの方向
	t_vec3 		direction;
}				t_lightling;

// 交点を表す構造体
typedef struct	s_intersection {
	// 交点までの距離
	double		distance;
	// 交点の位置
	t_vec3		position;
	// 交点における法線ベクトル
	t_vec3		normal;
}				t_intersection;

enum	e_shape {
		PLANE,
		SPHERE,
		SQUARE,
		CYLINDER,
		TRIANGLE
};

typedef struct		s_material {
	t_fcolor		kAmb;      // 環境光反射係数
	t_fcolor		kDif;      // 拡散反射係数
	t_fcolor		kSpe;      // 鏡面反射係数
	float			shininess; // 光沢度
}					t_material;

typedef struct		s_object {
	enum e_shape	type;
	t_vec3			center;  // plane, sphere
	t_vec3			normal;  // plane
	double			radius;  // sphere
	t_material		material;  // 材料の反射係数などを保持する
}					t_object;

typedef struct	s_world {
	void		*mlx;
	void		*win;
	t_img		img;
	int			screen_width;
	int			screen_height;
	t_list		*objects;
}				t_world;

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
// Objects
t_object		*sphere_init(t_vec3 center, double radius, t_material material);
t_object		*plane_init(t_vec3 center, t_vec3 normal, t_material material);
// Object's functions
bool			sphere_has_intersection(t_ray ray, t_object sphere);
t_intersection	calc_sphere_intersection(t_ray ray, t_object sphere);
bool			plane_has_intersection(t_ray ray, t_object plane);
t_intersection	calc_plane_intersection(t_ray ray, t_object plane);
// calculations
bool			has_intersection(t_ray ray, t_object object);
t_intersection	calc_intersection(t_ray ray, t_object object);
t_object		*get_nearest_object(t_world *world, t_ray ray);
// fcolor
uint32_t		fcolor2hex(t_fcolor fcolor);
t_fcolor		fcolor_init(double red, double green, double blue);
t_fcolor		fcolor_add(t_fcolor a, t_fcolor b);
t_fcolor		fcolor_mult(t_fcolor a, t_fcolor b);
t_fcolor		fcolor_mult_scalar(t_fcolor a, double b);
// material
t_material		material_init(t_fcolor kAmb, t_fcolor kDif, t_fcolor kSpe);
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
// Debug
void			print_world(t_world *world);
char			*get_type_name(enum e_shape type);
void			print_vec3(t_vec3 vec);
void			print_object(t_object object);
#endif
