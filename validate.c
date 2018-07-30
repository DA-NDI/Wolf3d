/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolgin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/31 12:36:22 by avolgin           #+#    #+#             */
/*   Updated: 2018/07/14 21:44:12 by avolgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

int		check_dimensions_and_symbols(char **map, int height, int width)
{
	int		i;
	int		j;
	int		dot;

	dot = 0;
	i = -1;
	while (++i < height)
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == '0')
				dot++;
			if (map[i][j] != '0' && map[i][j] != '1' && map[i][j] != '2' &&\
				map[i][j] != '\0' && map[i][j] != '3' && map[i][j] != 'P' &&\
				map[i][j] != '4' && map[i][j] != '5' && map[i][j] != 'A' &&
				map[i][j] != '7' && map[i][j] != '8')
				return (1);
		}
		if (j != width && (map[i][0] != '\0' || map[i][j] != '\0'))
			return (1);
	}
	return (i != height || !dot) ? 1 : 0;
}

int		check_boundaries(char **map, int height)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (map[i][j] && map[height - 1][j])
		if (map[i][j] != '1' || map[height - 1][j++] != '1')
			return (1);
	j = 0;
	while (++i < height)
	{
		if (map[i][j] != '1')
			return (1);
		while (map[i][j++])
			;
		if (map[i][j - 2] != '1')
			return (1);
		j = 0;
	}
	if (map[i][j] != '\0')
		return (1);
	return (0);
}

void	ft_verify_map(char **map, t_wolf *holder)
{
	int		i;
	int		j;

	j = 0;
	i = 0;
	if (!map || map[0][0] == '\n' || map[0][0] != '1')
		ft_print_error("Map error");
	while (map[i] != '\0')
		i++;
	while (map[0][j] != '\0')
		j++;
	holder->map_width = j - 1;
	holder->map_height = i - 2;
	if (i < 3 || j < 3)
		ft_print_error("Map is too small");
	if (j > 50)
		ft_print_error("Map is too wide");
	if (check_boundaries(map, i - 1))
		ft_print_error("Boundaries error. Should be all '1'");
	if (check_dimensions_and_symbols(map, i - 1, j))
		ft_print_error("Map dimensions or allowed symbols error");
}

char	**ft_create_map(char **argv)
{
	int			fd;
	char		*buff;
	static char	*map[50];
	int			i;

	i = 0;
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		ft_print_error("Failed to open map file");
	while (get_next_line(fd, &buff) == 1)
	{
		if (ft_strlen(buff) > 50 || i >= 50)
		{
			free(buff);
			ft_print_error("map is too big!");
		}
		map[i++] = buff;
	}
	map[i] = NULL;
	if (i == 0)
		ft_print_error("Map is empty!");
	i = -1;
	return (map);
}
