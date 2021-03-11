#include "minirt.h"

bool		is_valid_rtpath(char *path)
{
	size_t	path_len;

	// cubfileの名前が正しいかチェックする(*.cubか)
	path_len = ft_strlen(path);
	return !(path_len < 4 || path[path_len - 4] == '/'
			|| ft_strncmp(path + path_len - 3, ".rt", 3));
}

int			load_cubfile_fd(t_world *world, int fd)
{
	char	*line;
	int		status;
	char	**params;

	status = 0;
	while (status >= 0 && (status = get_next_line(fd, &line)) == 1)
	{
		status = !(params = ft_split(line, ' ')) ? ERROR : status;
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
	status = load_cubfile_fd(world, fd);
	return (status);
}

