#include "minirt.h"

t_material	material_init(t_fcolor kDif, t_fcolor kSpe, double shininess)
{
	t_material material;
	// 拡散反射係数k_d
	material.kDif = kDif;
	// 鏡面反射係数k_s
	material.kSpe = kSpe;
	// 光沢度α
	material.shininess = shininess;
	return (material);
}

