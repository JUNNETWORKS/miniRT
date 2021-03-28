#include "minirt.h"

bool		has_shadow(t_world *world, t_light light, t_intersection intersection)
{
	// 交点から光源までのベクトルの計算
	t_vec3 intersection2light;
	intersection2light = vec3_sub(light.position, intersection.position);
	// 入射ベクトル
	t_vec3 l = vec3_normalize(intersection2light);
	// シャドウレイ
	// 始点は微小距離ずらすことで物体自身と交差が出ないようにする
	t_ray shadow_ray;
	shadow_ray.start = vec3_add(intersection.position, vec3_mult(l, EPSILON));
	shadow_ray.direction = l;
	// 交点から光源までの距離
	double light_dist = vec3_mag(intersection2light) - EPSILON;
	// シャドウベクトルが交点とぶつかる&&light_distより小さい ということは交点と光源との間に物体があるということ
	t_list *current_object = world->objects;
	while (current_object)
	{
		t_intersection inter = calc_intersection(shadow_ray, *(t_object*)current_object->content);
		if (inter.has_intersection && inter.distance >= 0 && inter.distance <= light_dist)
		{
			return (true);
		}
		current_object = current_object->next;
	}
	return (false);
}

// 拡散反射光(Diffuse) の計算
t_fcolor	calc_ref_diff(t_ray ray, t_object object, t_intersection intersection, t_light light)
{
	// 光の入射ベクトルの計算
	t_vec3 l;
	// 法線ベクトルと光源の入射ベクトルが成す角
	double ray_deg;
	// 拡散反射光の放射輝度R_d
	t_fcolor ref_diff;

	l = vec3_normalize(vec3_sub(light.position, intersection.position));
	ray_deg = vec3_dot(intersection.normal, l);
	// 法線ベクトルと入射ベクトルの成す角がπ/2を超えた時, 光は裏側から当たっているので反射は起きない
	if (ray_deg < 0)
		ray_deg = 0;
	// 拡散反射光R_dの計算
	ref_diff = fcolor_mult_scalar(fcolor_mult(object.material.kDif, light.intensity), ray_deg);
	return (ref_diff);
}

// 鏡面反射光(Specular) の計算
t_fcolor	calc_ref_spec(t_ray ray, t_object object, t_intersection intersection, t_light light)
{
	// 光の入射ベクトルの計算
	t_vec3 l;
	// 視線ベクトルの逆ベクトル
	t_vec3 v;
	// 入射光の正反射ベクトル
	t_vec3 r;
	t_fcolor ref_spec;

	l = vec3_normalize(vec3_sub(light.position, intersection.position));
	// 拡散反射光R_dの計算
	v = vec3_mult(ray.direction, -1);
	r = vec3_sub(vec3_mult(vec3_mult(intersection.normal, vec3_dot(intersection.normal, l)), 2), l);
	ref_spec = fcolor_mult_scalar(fcolor_mult(object.material.kSpe, light.intensity), pow(vec3_dot(v, r), object.material.shininess));
	if (vec3_dot(v, r) < 0)
		ref_spec = fcolor_init(0, 0, 0);
	return (ref_spec);
}
