/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avolgin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/14 20:57:05 by avolgin           #+#    #+#             */
/*   Updated: 2018/07/20 09:51:01 by avolgin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wolf3d.h>

#define FULLSCREEN SDL_WINDOW_FULLSCREEN
#define OPENGL SDL_WINDOW_OPENGL
#define C_Q (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
#define PLANE_Y camera->plane_y
#define ROT_SPEED camera->rot_speed
#define WINDOW holder->window
#define MOV_SP camera->move_speed

void		ft_check_left_right(t_camera *camera, float old, \
								const Uint8 *keystate)
{
	float old_plane_x;

	if (keystate[SDL_SCANCODE_RIGHT])
	{
		old = DIR_X;
		DIR_X = DIR_X * cos(ROT_SPEED) - DIR_Y * sin(ROT_SPEED);
		DIR_Y = old * sin(ROT_SPEED) + DIR_Y * cos(ROT_SPEED);
		old_plane_x = PLANE_X_COORD;
		PLANE_X_COORD = PLANE_X_COORD * cos(ROT_SPEED) - \
			camera->plane_y * sin(ROT_SPEED);
		camera->plane_y = old_plane_x * sin(ROT_SPEED) + \
			camera->plane_y * cos(ROT_SPEED);
	}
	if (keystate[SDL_SCANCODE_LEFT])
	{
		old = DIR_X;
		DIR_X = DIR_X * cos(-ROT_SPEED) - DIR_Y * sin(-ROT_SPEED);
		DIR_Y = old * sin(-ROT_SPEED) + DIR_Y * cos(-ROT_SPEED);
		old_plane_x = PLANE_X_COORD;
		PLANE_X_COORD = PLANE_X_COORD * cos(-ROT_SPEED) - \
			camera->plane_y * sin(-ROT_SPEED);
		PLANE_Y = old_plane_x * sin(-ROT_SPEED) + PLANE_Y * cos(-ROT_SPEED);
	}
}

/*
**		hide mouse:
**		if (SDL_GetRelativeMouseMode() == SDL_TRUE)
**			SDL_SetRelativeMouseMode(SDL_FALSE);
**		else
**   SDL_SetRelativeMouseMode(SDL_TRUE);
*/

void		ft_check_pause(t_wolf *holder, const Uint8 *keystate)
{
	if (keystate[SDL_SCANCODE_P] && !holder->starting)
	{
		SDL_WarpMouseInWindow(WINDOW, WIDTH / 2, HEIGHT / 2);
		draw_text(holder, "Pause", WIDTH / 2, 10);
		SDL_RenderPresent(holder->renderer);
		holder->pause = (holder->pause == -1) ? 0 : 1;
		if (Mix_PausedMusic() == 0 && holder->pause)
			Mix_PauseMusic();
		else if (!holder->pause)
			Mix_ResumeMusic();
	}
}

void		ft_check_other(t_wolf *holder, const Uint8 *keystate)
{
	if (keystate[SDL_SCANCODE_DOWN] && holder->starting)
	{
		SDL_RenderCopy(holder->renderer, START_SCR, NULL, NULL);
		draw_text(holder, "	  Play", WIDTH / 2 - 10, HEIGHT / 2);
		draw_text(holder, " > Quit", WIDTH / 2 - 10, HEIGHT / 2 + 15);
		draw_text(holder, "PRESS ENTER TO SELECT", WIDTH / 2, HEIGHT - 25);
		holder->start->play_select = 0;
		SDL_RenderPresent(holder->renderer);
	}
	if (keystate[SDL_SCANCODE_UP] && holder->starting)
	{
		SDL_RenderCopy(holder->renderer, START_SCR, NULL, NULL);
		draw_text(holder, " > Play", WIDTH / 2 - 10, HEIGHT / 2);
		draw_text(holder, "	  Quit", WIDTH / 2 - 10, HEIGHT / 2 + 15);
		draw_text(holder, "PRESS ENTER TO SELECT", WIDTH / 2, HEIGHT - 25);
		holder->start->play_select = 1;
		SDL_RenderPresent(holder->renderer);
	}
}

void		check_up_d(t_wolf *holder, t_camera *camera, const Uint8 *keystate)
{
	if (keystate[SDL_SCANCODE_UP])
	{
		MOV_SP = (!keystate[SDL_SCANCODE_LSHIFT]) ? 0.2 : 0.5;
		if (MAP[(int)(P_Y)][(int)(P_X + DIR_X * MOV_SP)] == '2' && \
			MAP[(int)(P_Y)][(int)(P_X + DIR_X * MOV_SP) + 2] == '0')
		{
			P_X += DIR_X * MOV_SP + 2;
			Mix_PlayChannel(-1, camera->dstelept, 0);
		}
		else if (MAP[(int)(P_Y)][(int)(P_X + DIR_X * MOV_SP)] == '0' \
			|| MAP[(int)(P_Y)][(int)(P_X + DIR_X * MOV_SP)] == 'P')
			P_X += DIR_X * MOV_SP;
		if (MAP[(int)(P_Y + DIR_Y * MOV_SP)][(int)(P_X)] == '0' \
			|| MAP[(int)(P_Y + DIR_Y * MOV_SP)][(int)(P_X)] == 'P')
			P_Y += DIR_Y * MOV_SP;
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		if (MAP[(int)(P_Y)][(int)(P_X - DIR_X * MOV_SP)] == '0' \
			|| MAP[(int)(P_Y)][(int)(P_X - DIR_X * MOV_SP)] == 'P')
			P_X -= DIR_X * MOV_SP;
		if (MAP[(int)(P_Y - DIR_Y * MOV_SP)][(int)(P_X)] == '0' \
			|| MAP[(int)(P_Y - DIR_Y * MOV_SP)][(int)(P_X)] == 'P')
			P_Y -= DIR_Y * MOV_SP;
	}
}

void		ft_close_loop(t_wolf *holder, t_camera *camera)
{
	SDL_Event	event;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || C_Q)
			ft_close(holder);
		check_up_d(holder, camera, keystate);
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
		{
			ft_check_space(holder, keystate);
			ft_check_other(holder, keystate);
			ft_check_extra_keys(holder, keystate);
			ft_check_pause(holder, keystate);
		}
		ft_check_left_right(camera, 0, keystate);
	}
}
