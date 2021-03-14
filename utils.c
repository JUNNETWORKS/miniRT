#include "minirt.h"

double	deg2rad(int x)
{
	return ((double)x / 180 * M_PI);
}

int	rad2deg(double x)
{
	return (int)(x * 180 / M_PI);
}

bool		str_all_true(char *str, int(*f)(int))
{
	int		i;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		if (!f(str[i]))
			return (false);
		i++;
	}
	return (true);
}

size_t		str_c_count(char *str, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while(str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

double		ft_atof(char *str)
{
	double	ans;
	char	**nums;

	if (!str || get_split_size(str, '.') > 2)
		return 0;
	nums = ft_split(str, '.');
	ans = 0;
	// 整数部分
	size_t i = 0;
	while (i < ft_num_len(nums[0]) && ft_isdigit(nums[0][i]))
	{
		ans += (nums[0][i] - '0') * pow(10, ft_num_len(nums[0]) - i - 1);
		i++;
	}
	if (ptrarr_len((void**)nums) == 1)
		return (ans);
	i = 0;
	while(i < ft_num_len(nums[1]) && ft_isdigit(nums[1][i]))
	{
		ans += (nums[1][i] - '0') / pow(10, i + 1);
		i++;
	}
	free_ptrarr((void**)nums);
	return (ans);
}





