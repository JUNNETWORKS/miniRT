#ifndef VEC3_H
# define VEC3_H

// Vector3D
typedef struct	s_vec3 {
	double		x;
	double		y;
	double		z;
} t_vec3;

// Vec3 Utils
t_vec3			vec3_init(double x, double y, double z);
t_vec3			vec3_add(t_vec3 a, t_vec3 b);
t_vec3			vec3_sub(t_vec3 a, t_vec3 b);
t_vec3			vec3_mult(t_vec3 a, double b);
double			vec3_dot(t_vec3 a, t_vec3 b);
t_vec3			vec3_cross(t_vec3 a, t_vec3 b);
double			vec3_mag(t_vec3 a);
t_vec3			vec3_normalize(t_vec3 a);
int				get_vec3_from_str(t_vec3 *vec, char *str);

#endif
