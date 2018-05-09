#include <iostream>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_image.h>
#include <math.h>

using namespace std;

int wallCollide(int x, int y, int dir, int level[20][20]);

//constants: not supposed to change execept here!
const float FPS = 60;
const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const int PacMan_SIZE = 32;
const int PACSIZE = 32;

//it helps you be more lazy for the one to move things
enum keyz { KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN };
enum direction { RIGHT, LEFT, UP, DOWN };
ALLEGRO_BITMAP *PacMan = NULL;
ALLEGRO_BITMAP *wall = NULL;
ALLEGRO_BITMAP *food = NULL;
float PacMan_dx = -4.0, PacMan_dy = 4.0;
float PacMan_x = 470;
float PacMan_y = 480;


bool keyz[4] = { false, false, false, false };
bool redraw = true;
bool doexit = false;

al_init();
al_install_keyboard();

timer = al_create_timer(1.0 / FPS);

display = al_create_display(SCREEN_W, SCREEN_H);

PacMan = al_create_bitmap(PacMan_SIZE, PacMan_SIZE);
al_set_target_bitmap(PacMan);
al_clear_to_color(al_map_rgb(255, 0, 255));

wall = al_create_bitmap(50, 50);
al_set_target_bitmap(wall);
al_clear_to_color(al_map_rgb(0, 0, 255));

food = al_create_bitmap(20, 20);
al_set_target_bitmap(food);
al_clear_to_color(al_map_rgb(255, 255, 0));

al_set_target_bitmap(al_get_backbuffer(display));

event_queue = al_create_event_queue();

al_register_event_source(event_queue, al_get_display_event_source(display));
al_register_event_source(event_queue, al_get_timer_event_source(timer));
al_register_event_source(event_queue, al_get_keyboard_event_source());

al_clear_to_color(al_map_rgb(0, 0, 0));

al_flip_display();

al_start_timer(timer);

while (!doexit)
{
	//cout << "game loop" << endl;
	ALLEGRO_EVENT ev;
	al_wait_for_event(event_queue, &ev);

	if (ev.type == ALLEGRO_EVENT_TIMER) {
		//cout << "timer section" << endl;
		if (keyz[KEY_UP]) {
			PacMan_y -= 4.0;

		}

		if (keyz[KEY_DOWN]) {
			PacMan_y += 4.0;
		}

		if (keyz[KEY_LEFT] && !wallCollide(PacMan_x, PacMan_y, LEFT, level)) {
			PacMan_x -= 4.0;
		}

		if (keyz[KEY_RIGHT] && !wallCollide(PacMan_x, PacMan_y, RIGHT, level)) {
			cout << "moving right" << endl;

			PacMan_x += 4.0;
		}


		redraw = true;
	}
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		break;
	}

	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (ev.keyboard.keycode) {
		case ALLEGRO_KEY_UP:
			keyz[KEY_UP] = true;
			break;

		case ALLEGRO_KEY_DOWN:
			keyz[KEY_DOWN] = true;
			break;

		case ALLEGRO_KEY_LEFT:
			keyz[KEY_LEFT] = true;
			break;

		case ALLEGRO_KEY_RIGHT:
			keyz[KEY_RIGHT] = true;
			cout << "right key pressed!" << endl;
			break;
		}
	}

	else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
		switch (ev.keyboard.keycode) {
		case ALLEGRO_KEY_UP:
			keyz[KEY_UP] = false;
			break;

		case ALLEGRO_KEY_DOWN:
			keyz[KEY_DOWN] = false;
			break;

		case ALLEGRO_KEY_LEFT:
			keyz[KEY_LEFT] = false;
			break;

		case ALLEGRO_KEY_RIGHT:
			keyz[KEY_RIGHT] = false;
			break;

		case ALLEGRO_KEY_ESCAPE:
			doexit = true;
			break;
		}
	}

	if (redraw && al_is_event_queue_empty(event_queue)) {
		redraw = false;

		al_clear_to_color(al_map_rgb(0, 0, 0));
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 20; j++) {
				if (level[i][j] == 1)
					al_draw_bitmap(wall, i * 50, j * 50, 0);

				else if (level[i][j] == 0)
					al_draw_bitmap(food, i * 50 + 15, j * 50 + 15, 0);

			}
		al_draw_bitmap(PacMan, PacMan_x, PacMan_y, 0);

		al_flip_display();
	}
}

al_destroy_bitmap(PacMan);
al_destroy_timer(timer);
al_destroy_display(display);
al_destroy_event_queue(event_queue);

return 0;
}


int wallCollide(int x, int y, int dir, int level[20][20]) {
	cout << "calling collision function";
	if (dir == RIGHT) {
		cout << "checking right collision" << endl;
		int new_x1 = x + 3 + PACSIZE;
		int new_x2 = x + 3 + PACSIZE;
		int new_x3 = x + 3 + PACSIZE;

		int new_y1 = y;
		int new_y2 = y + PACSIZE / 2;
		int new_y3 = y + PACSIZE;
		cout << "right collision if statement" << endl;
		if ((level[new_x1 / 50][new_y1 / 50] == 1) || (level[new_x2 / 50][new_y2 / 50]) || (level[new_x3 / 50][new_y3 / 50] == 1)) {
			cout << "right collision!" << endl;
			return 1;
		}
		else
			return 0;
	}

	else if (dir == LEFT) {
		int new_y1 = y;
		int new_y2 = y + PACSIZE / 2;
		int new_y3 = y + PACSIZE;

		int new_x1 = x - 3;
		int new_x2 = x - 3;
		int new_x3 = x - 3;

		if ((level[new_y1 / 50][new_x1 / 50] == 1) || (level[new_y2 / 50][new_x2 / 50]) || (level[new_y3 / 50][new_x3 / 50] == 1))

			return 1;
		else
			return 0;
	}
}