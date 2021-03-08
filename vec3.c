#include "minirt.h"

// ベクトルの加算
t_vec3	*vec3_add(t_vec3 a, t_vec3 b)
{
	t_vec3 *new;
	if (!(new = malloc(sizeof(t_vec3))))
		return (NULL);
	new->x = a.x + b.x;
	new->y = a.y + b.y;
	new->z = a.z + b.z;
	return (new);
}

// ベクトルの減算
t_vec3	*vec3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3 *new;
	if (!(new = malloc(sizeof(t_vec3))))
		return (NULL);
	new->x = a.x - b.x;
	new->y = a.y - b.y;
	new->z = a.z - b.z;
	return (new);
}

// ベクトルのスカラー倍
t_vec3	*vec3_mult(t_vec3 a, double b)
{
	t_vec3 *new;
	if (!(new = malloc(sizeof(t_vec3))))
		return (NULL);
	new->x = a.x * b;
	new->y = a.y * b;
	new->z = a.z * b;
	return (new);
}

// ベクトルの内積
double	vec3_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// ベクトルの外積
t_vec3	*vec3_cross(t_vec3 a, t_vec3 b)
{
	t_vec3 *new;
	if (!(new = malloc(sizeof(t_vec3))))
		return (NULL);
	new->x = a.y * b.z - a.z - b.y;
	new->y = a.z * b.x - a.x - b.z;
	new->z = a.x * b.y - a.y * b.x;
	return (new);
}

// ベクトルのノルム(長さ)
double	vec3_mag(t_vec3 a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}
