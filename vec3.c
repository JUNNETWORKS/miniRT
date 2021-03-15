#include "minirt.h"

t_vec3	vec3_init(double x, double y, double z)
{
	t_vec3 new;
	new.x = x;
	new.y = y;
	new.z = z;
	return (new);
}

// ベクトルの加算
t_vec3	vec3_add(t_vec3 a, t_vec3 b)
{
	return (vec3_init(a.x + b.x, a.y + b.y, a.z + b.z));
}

// ベクトルの減算
t_vec3	vec3_sub(t_vec3 a, t_vec3 b)
{
	return (vec3_init(a.x - b.x, a.y - b.y, a.z - b.z));
}

// ベクトルのスカラー倍
t_vec3	vec3_mult(t_vec3 a, double b)
{
	return (vec3_init(a.x * b, a.y * b, a.z * b));
}

// ベクトルの内積
double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// ベクトルの外積
t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	return (vec3_init(a.y * b.z - a.z - b.y, a.z * b.x - a.x - b.z, a.x * b.y - a.y * b.x));
}

// ベクトルのノルム(長さ)
double	vec3_mag(t_vec3 a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

// ベクトルの正規化
t_vec3	vec3_normalize(t_vec3 a)
{
	double mag = vec3_mag(a);
	return (vec3_init(a.x / mag, a.y / mag, a.z / mag));
}

// 文字列をパースして*vecに値をセットする
int		get_vec3_from_str(t_vec3 *vec, char *str)
{
	char **xyz;

	xyz = NULL;
	if (!(xyz = ft_split(str, ',')) || ptrarr_len((void**)xyz) != 3)
	{
		printf("input str: %s\n", str);
		free_ptrarr((void**)xyz);
		return (put_and_return_err("failed parse string to vector"));
	}
	vec->x = ft_atof(xyz[0]);
	vec->y = ft_atof(xyz[1]);
	vec->z = ft_atof(xyz[2]);
	free_ptrarr((void**)xyz);
	return (0);
}
