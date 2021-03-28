#include "minirt.h"

static void	write_file_header(t_img *img, int fd)
{
	uint32_t	tmp;

	write(fd, "BM", 2);
	tmp = 14 + 10 + img->width * img->height * 4;
	write(fd, &tmp, 4);
	write(fd, "\0\0", 2);
	write(fd, "\0\0", 2);
	tmp = 14 + 10;
	write(fd, &tmp, 4);
}

static void	write_information_header(t_img *img, int fd)
{
	uint32_t	tmp;

	tmp = 12;
	write(fd, &tmp, 4);
	write(fd, &img->width, 2);
	write(fd, &img->height, 2);
	tmp = 1;
	write(fd, &tmp, 2);
	tmp = 32;
	write(fd, &tmp, 2);
}

static void	write_image_data(t_img *img, int fd)
{
	uint32_t	tmp;
	int			x;
	int			y;

	y = img->height - 1;
	while (y >= 0)
	{
		x = 0;
		while (x < img->width)
		{
			tmp = get_color_from_img(*img, x, y);
			write(fd, &tmp, 4);
			x++;
		}
		y--;
	}
}

int			write_camera2bmp(t_camera *camera, char *fpath)
{
	int			fd;

	if ((fd = open(fpath, O_WRONLY | O_CREAT,
			S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (-1);
	write_file_header(&camera->img, fd);
	write_information_header(&camera->img, fd);
	write_image_data(&camera->img, fd);
	close(fd);
	return (0);
}

int			write_world2bmp(t_world *world)
{
	t_dlist		*first_camera;  // ループ用
	t_camera	*camera;
	char		filepath[256];
	int			idx;

	first_camera = world->cameras;
	idx = 0;
	do {
		camera = (t_camera*)world->cameras->content;
		sprintf(filepath, "camera_%d.bmp", idx);
		write_camera2bmp(camera, filepath);
		printf("DONE writing bmp file %s\n", filepath);
		world->cameras = world->cameras->next;
		idx++;
	} while(world->cameras != first_camera);
	printf("DONE writing all bmp files\n");
	return (0);
}
