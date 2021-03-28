#ifndef MINIRT_H
# define MINIRT_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include "vec3.h"
# include "dlist.h"
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

// Key definitions
# define KEY_q 113
# define KEY_esc 65307
# define KEY_w 119
# define KEY_a 97
# define KEY_s 115
# define KEY_d 100
# define KEY_rallow 65363
# define KEY_lallow 65361

# define EPSILON 1.0 / 512
// 光沢度α
# define SHININESS 30

# define MIN(x, y) ((x) < (y) ? (x) : (y))

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

// 光源を表す構造体
typedef struct	s_light {
	// 光源の位置
	t_vec3		position;
	// ライティングの強度
	t_fcolor	intensity;
}				t_light;

// 交点を表す構造体
typedef struct	s_intersection {
	// 交点が存在するか
	bool		has_intersection;
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
	t_fcolor		kDif;      // 拡散反射係数  // これを物体の色とする
	t_fcolor		kSpe;      // 鏡面反射係数
	float			shininess; // 光沢度
}					t_material;

typedef struct		s_object {
	enum e_shape	type;
	t_vec3			center;  // plane, sphere, square, cylinder
	t_vec3			normal;  // plane, square, cylinder
	double			radius;  // sphere, cylinder
	double			side_size;  // square
	double			height;     // cylinder
	t_material		material;  // 材料の反射係数などを保持する
	t_vec3			p1;      // triangle
	t_vec3			p2;      // triangle
	t_vec3			p3;      // triangle
}					t_object;

typedef struct		s_camera {
	t_vec3			pos;
	t_vec3			orientation;
	double			fov;
	t_vec3			x_basis;
	t_vec3			y_basis;
	t_vec3			d_center;  // カメラからスクリーンの中心へのベクトル
	t_img			img;
}					t_camera;

typedef struct	s_world {
	void		*mlx;
	void		*win;
	t_img		img;
	int			screen_width;
	int			screen_height;
	t_dlist		*cameras;
	t_list		*objects;
	t_fcolor	ambient;  // 環境光の強度
	t_list		*lights;
}				t_world;

// MLX Utils
void			my_mlx_pixel_put(t_img *img, int x, int y, int color);
uint32_t		get_color_from_img(t_img img, int x, int y);
void			clear_img(t_img *img);
// Initializers
t_object		*sphere_init(t_vec3 center, double radius, t_material material);
t_object		*plane_init(t_vec3 center, t_vec3 normal, t_material material);
t_object		*triangle_init(t_vec3 p1, t_vec3 p2, t_vec3 p3, t_material material);
t_object		*square_init(t_vec3 center, t_vec3 normal, double side_size, t_material material);
t_object		*cylinder_init(t_vec3 point, t_vec3 normal, double radius, double height, t_material material);
t_light			*light_init(t_vec3 position, t_fcolor intensity);
t_camera		*camera_init(t_world *world, t_vec3 pos, t_vec3 orientation, double fov);
// Object's functions
t_intersection	calc_sphere_intersection(t_ray ray, t_object sphere);
t_intersection	calc_plane_intersection(t_ray ray, t_object plane);
t_intersection	calc_triangle_intersection(t_ray ray, t_object triangle);
t_intersection	calc_square_intersection(t_ray ray, t_object square);
t_intersection	calc_cylinder_intersection(t_ray ray, t_object cylinder);
// calculations
t_intersection	calc_intersection(t_ray ray, t_object object);
t_object		*get_nearest_object(t_world *world, t_ray ray);
t_fcolor		calc_ref_diff(t_ray ray, t_object object, t_intersection intersection, t_light light);
t_fcolor		calc_ref_spec(t_ray ray, t_object object, t_intersection intersection, t_light light);
bool			has_shadow(t_world *world, t_light light, t_intersection intersection);
// fcolor
uint32_t		fcolor2hex(t_fcolor fcolor);
t_fcolor		fcolor_normalize(t_fcolor fcolor);
t_fcolor		fcolor_init(double red, double green, double blue);
t_fcolor		fcolor_add(t_fcolor a, t_fcolor b);
t_fcolor		fcolor_mult(t_fcolor a, t_fcolor b);
t_fcolor		fcolor_mult_scalar(t_fcolor a, double b);
int				get_fcolor_from_rgbstr(t_fcolor *fcolor, char *rgbstr);
// material
t_material		material_init(t_fcolor kDif, t_fcolor kSpe, double shininess);
// hooks
int				key_press_hook(int keycode, t_world *world);
int				exit_world(t_world *world);
// load rt file
int				load_rtfile(t_world *world, char *path);
int				set_resolution(t_world *game, char *width_str, char *height_str);
// world
int				initialize_world(t_world *world);
int				configure_window(t_world *world);
int				configure_screen(t_world *world, bool has_window);
// BMP
int				write_camera2bmp(t_camera *camera, char *fpath);
int				write_world2bmp(t_world *world);
// Utils
double			deg2rad(double x);
int				rad2deg(double x);
uint32_t		rgb2hex(int r, int g, int b);
uint32_t		alpha_blend(uint32_t dst, uint32_t src);
uint32_t		get_rgbhex_from_rgbstr(uint32_t *color, char *rgbstr);
void			put_error_msg(char *str);
int				put_and_return_err(char *str);
int				put_and_exit_err(char *str);
bool			str_all_true(char *str, int(*f)(int));
size_t			str_c_count(char *str, char c);
size_t			ptrarr_len(void **ptrarr);
void			free_ptrarr(void **ptrarr);
void			free_ptrarr_and_assign_null(void ***ptrarr);
void			free_and_assign_null(void **p);
double			ft_atof(char *str);
// Debug
void			print_world(t_world *world);
char			*get_type_name(enum e_shape type);
void			print_vec3(t_vec3 vec);
void			print_object(t_object object);
#endif
