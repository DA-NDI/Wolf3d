/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolgin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/15 18:44:23 by avolgin           #+#    #+#             */
/*   Updated: 2018/07/20 06:03:49 by avolgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

unsigned int	get_pixel(SDL_Surface *surface, int x, int y)
{
	unsigned int	*pixels;

	pixels = (unsigned int*)surface->pixels;
	return (pixels[(y * surface->w) + x]);
}

/*
** To get 60 fps
**	if (holder->frame_time > holder->frame_delay)
**	{
**		SDL_Delay(holder->frame_time - holder->frame_delay);
**		fps = ft_strdup("60");
**	}
**	else
*/

void			draw_fps(t_wolf *holder, TTF_Font *font)
{
	char		*fps;
	char		*tmp;
	SDL_Surface	*surf;
	SDL_Texture	*texture;

	holder->frame_time = SDL_GetTicks() - holder->frame_start;
	fps = ft_itoa(1000 / holder->frame_time);
	tmp = fps;
	fps = ft_strjoin("FPS: ", fps);
	free(tmp);
	surf = TTF_RenderText_Solid(font, fps, WHITE);
	free(fps);
	texture = SDL_CreateTextureFromSurface(holder->renderer, surf);
	SDL_RenderCopy(holder->renderer, texture, NULL, &(SDL_Rect){5, 5, 56, 14});
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);
	draw_score(holder);
}

void			draw_text(t_wolf *holder, char *text, int x, int y)
{
	SDL_Surface	*surf;
	SDL_Texture	*texture;
	SDL_Rect	dest;

	surf = TTF_RenderText_Solid(holder->camera->font, text, WHITE);
	texture = SDL_CreateTextureFromSurface(holder->renderer, surf);
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	dest.x = x - (dest.w >> 1);
	dest.y = y;
	SDL_RenderCopy(holder->renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);
}

void			draw_floor1(t_wolf *holder, \
unsigned int buffer[][holder->width], unsigned int x, float dist_player)
{
	int		i;
	float	current_dist;
	float	weight;
	float	current_floor_x;
	float	current_floor_y;

	i = holder->camera->draw_end;
	dist_player = 0.0;
	while (++i < holder->height)
	{
		current_dist = FIXED_F(FIX_DIV(INT_F(holder->height), \
							INT_F((i << 1) - holder->height)));
		weight = (current_dist - dist_player) / (PERP_DIST_WALL - dist_player);
		current_floor_x = weight * WALL_X_FL + (1.0 - weight) * P_X;
		current_floor_y = weight * WALL_Y_FL + (1.0 - weight) * P_Y;
		buffer[i][x] = get_pixel(holder->camera->texture[4], \
							FLOOR_TEX_X >> 1, FLOOR_TEX_Y);
	}
}

void			draw_floor(t_wolf *holder, t_camera *camera, \
					unsigned int buffer[][holder->width], unsigned int x)
{
	if (camera->side == 0 && camera->ray_dir_x > 0)
	{
		camera->wall_x_f = camera->map_x;
		camera->wall_y_f = camera->map_y + camera->wall_x;
	}
	else if (camera->side == 0 && camera->ray_dir_x < 0)
	{
		camera->wall_x_f = camera->map_x + 1.0;
		camera->wall_y_f = camera->map_y + camera->wall_x;
	}
	else if (camera->side == 1 && camera->ray_dir_y > 0)
	{
		camera->wall_x_f = camera->map_x + camera->wall_x;
		camera->wall_y_f = camera->map_y;
	}
	else
	{
		camera->wall_x_f = camera->map_x + camera->wall_x;
		camera->wall_y_f = camera->map_y + 1.0;
	}
	if (camera->draw_end < 0)
		camera->draw_end = holder->height;
	draw_floor1(holder, buffer, x, 0.0);
}
