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
const int BOUNCER_SIZE = 32;
const int PACSIZE = 32;

//it helps you be more lazy for the one to move things
enum dir { KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN };



int main() {
	int level[20][20] = {
		1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,
		1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,1,1,0,1,1,1,1,1,0,1,1,0,1,1,0,1,
		1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,
		1,0,0,0,1,0,1,0,1,1,1,1,0,1,1,0,1,1,1,1,
		1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,
		1,0,0,0,1,0,1,0,1,1,1,0,1,0,1,1,0,1,1,1,
		1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,
		1,0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,0,1,
		1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,0,
		1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,0,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,
		1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,
		1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,1,
		1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
		1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1};

	//print to testing window
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			cout << level[i][j];
		}
		cout << endl;
	}
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_BITMAP *wall = NULL;
	float bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
	float bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
	//float bouncer_dx = -4.0, bouncer_dy = 4.0;
	bool key[4] = { false, false, false, false };
	bool redraw = true;
	bool doexit = false;

	al_init();
	al_install_keyboard();

	timer = al_create_timer(1.0 / FPS);

	display = al_create_display(SCREEN_W, SCREEN_H);

	bouncer = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE);
	al_set_target_bitmap(bouncer);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	wall = al_create_bitmap(50, 50);
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(0, 0, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (dir[KEY_UP] && bouncer_y >= 4.0) {
				bouncer_y -= 4.0;
			}

			if (dir[KEY_DOWN] && bouncer_y <= SCREEN_H - BOUNCER_SIZE - 4.0) {
				bouncer_y += 4.0;
			}

			if (dir[KEY_LEFT] && bouncer_x >= 4.0) {
				bouncer_x -= 4.0;
			}

			if (dir[KEY_RIGHT] && bouncer_x <= SCREEN_W - BOUNCER_SIZE - 4.0) {
				bouncer_x += 4.0;
			}

			//bouncer_x += bouncer_dx;
			//bouncer_y += bouncer_dy;

			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			}
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
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
				}

			al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

			al_flip_display();
		}
	}

	al_destroy_bitmap(bouncer);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}


int wallCollide(int x, int y, int dir, int level[20][20]) {
	if (dir == KEY_RIGHT) {
		int new_x1 = x + 3 + PACSIZE;
		int new_x2 = x + 3 + PACSIZE;
		int new_x3 = x + 3 + PACSIZE;

		int new_y1 = y;
		int new_y2 = y + PACSIZE / 2;
		int new_y3 = y + PACSIZE;

		if (level[new_x1][new_y1] == 1)
			|| (level[new_x2][new_y2])
			|| (level[new_x3][new_y3] == 1))

			return 1;
		else 
			return 0;

	}
}