/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolgin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/14 18:24:47 by avolgin           #+#    #+#             */
/*   Updated: 2018/07/30 10:02:48 by avolgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

void		ft_close(t_wolf *holder)
{
	Mix_FreeChunk(holder->weapon[0]->pistol_shoot);
	Mix_FreeChunk(holder->weapon[1]->pistol_shoot);
	holder->weapon[0]->pistol_shoot = NULL;
	holder->weapon[1]->pistol_shoot = NULL;
	SDL_DestroyRenderer(holder->renderer);
	SDL_DestroyWindow(holder->window);
	holder->running = 0;
	free(holder->start);
	TTF_Quit();
	SDL_Quit();
}

void		ft_print_error(char *str)
{
	ft_putendl(str);
	exit(1);
}

t_wolf		*holder_init(t_wolf *holder)
{
	holder->height = 768;
	holder->width = 1024;
	holder->running = 1;
	holder->shooting = 0;
	holder->fps = 60;
	holder->pause = 0;
	holder->starting = 1;
	holder->frame_delay = 15;
	holder->skybox_num = 0;
	holder->gun = 0;
	holder->frags = 0;
	return (holder);
}

void		start_game(t_wolf *holder)
{
	holder_init(holder);
	get_player_coordinates(holder);
	ft_init_image(holder);
	ft_start_screen(holder);
	ft_raycasting(holder, 0);
}

int			main(int argc, char **argv)
{
	static t_wolf	holder[1];

	if (argc != 2 || !argv[1])
		ft_print_error("Usage: ./wolf3d <map>");
	holder->map = ft_create_map(argv);
	ft_verify_map(holder->map, holder);
	start_game(holder);
	return (0);
}
