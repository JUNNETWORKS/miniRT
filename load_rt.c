#include "minirt.h"

bool		is_valid_rtpath(char *path)
{
	size_t	path_len;

	// cubfileの名前が正しいかチェックする(*.cubか)
	path_len = ft_strlen(path);
	return !(path_len < 4 || path[path_len - 4] == '/'
			|| ft_strncmp(path + path_len - 3, ".rt", 3));
}

int			load_rtfile_fd(t_world *world, int fd)
{
	char	*line;
	int		status;
	char	**params;

	status = 0;
	while (status >= 0 && (status = get_next_line(fd, &line)) == 1)
	{
		status = !(params = ft_split(line, ' ')) ? ERROR : status;
		if (status >= 0 && params[0] &&
			ft_strnstr(params[0], "R", ft_strlen(params[0])))
			status = set_resolution(world, params[1], params[2]);
		else if ((status >= 0 && params[0]) &&
			(params[0][0] == 'A'))
			status = set_ambient(world);  // TODO: 環境光
		else if ((status >= 0 && params[0]) &&
			(params[0][0] == 'l'))
			status = set_light(world);  // TODO: 光源
		else if ((status >= 0 && params[0])&&
			params[0][0] == 'c')
			status = set_camera(world);  // TODO: Camera
		else if ((status >= 0 && params[0])&&
			ft_strncmp(params[0], "sp", 3))
			status = set_camera(world);  // TODO: Sphere
		else if ((status >= 0 && params[0])&&
			ft_strncmp(params[0], "pl", 3))
			status = set_camera(world);  // TODO: Plane
		else if ((status >= 0 && params[0])&&
			ft_strncmp(params[0], "sq", 3))
			status = set_camera(world);  // TODO: Square
		else if ((status >= 0 && params[0])&&
			ft_strncmp(params[0], "cy", 3))
			status = set_camera(world);  // TODO: Cylinder
		else if ((status >= 0 && params[0])&&
			ft_strncmp(params[0], "tr", 3))
			status = set_camera(world);  // TODO: Triangle
		free_and_assign_null((void**)&line);
		free_ptrarr((void**)params);
	}
	free(line);
	return (status);
}


int			load_rt(t_world *world, char *path)
{
	int		fd;
	int		status;

	if (!is_valid_rtpath(path))
		return (put_and_return_err("File extension is not .cub"));
	if ((fd = open(path, O_RDONLY)) == -1)
		return (put_and_return_err("Failed to open file"));
	status = load_rtfile_fd(world, fd);
	return (status);
}

