#include "minirt.h"

t_fcolor	calc_lighting_Rds(t_ray ray, t_object object, t_intersection intersection, t_light light)
{
				// 光の入射ベクトルの計算
				t_vec3 l;
				l = vec3_normalize(vec3_sub(light.position, intersection.position));

				double ray_deg = vec3_dot(intersection.normal, l);
				// 法線ベクトルと入射ベクトルの成す角がπ/2を超えた時, 光は裏側から当たっているので反射は起きない
				if (ray_deg < 0)
					ray_deg = 0;

				// 拡散反射光R_dの計算
				// 光源の強度I_i
				t_fcolor I_i = light.intensity;
				// 拡散反射係数k_d
				t_fcolor k_d = object.material.kDif;
				// 拡散反射光の放射輝度R_d
				t_fcolor R_d = fcolor_mult_scalar(fcolor_mult(k_d, I_i), ray_deg);

				// 鏡面反射光R_sの計算
				// 鏡面反射係数k_s
				t_fcolor k_s = object.material.kSpe;
				// 光沢度α
				double alpha = 8;
				// 視線ベクトルの逆ベクトル
				t_vec3 v = vec3_mult(ray.direction, -1);
				// 入射光の正反射ベクトル
				t_vec3 r = vec3_sub(vec3_mult(vec3_mult(intersection.normal, vec3_dot(intersection.normal, l)), 2), l);
				// 鏡面反射光の放射強度R_s
				t_fcolor R_s = fcolor_mult_scalar(fcolor_mult(k_s, I_i), pow(vec3_dot(v, r), alpha));
				if (vec3_dot(v, r) < 0)
					R_s = fcolor_init(0, 0, 0);

				// 最終的な輝度  (環境光 + 拡散反射光 + 鏡面反射光)
				t_fcolor R_ds = fcolor_add(R_d, R_s);
				return (R_ds);
}
