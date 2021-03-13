#include "minirt.h"

t_material	material_init(t_fcolor kAmb, t_fcolor kDif, t_fcolor kSpe)
{
	t_material material;
	// 環境光反射係数
	material.kAmb = kAmb;
	// 拡散反射係数k_d
	material.kDif = kDif;
	// 鏡面反射係数k_s
	material.kSpe = kSpe;
	return (material);
}

