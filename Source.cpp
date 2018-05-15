#include <iostream>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <math.h>

using namespace std;

int wallCollide(int x, int y, int dir, int level[20][20]);

//constants: not supposed to change execept here!
const float FPS = 60;
const int SCREEN_W = 1000;
const int SCREEN_H = 1000;
const int PacMan_SIZE = 32;
const int PACSIZE = 32;

class pac {
public:
	//ghost();
	void initGhost(int x, int y, char c, int dir, bool dead, int g);
	bool isdead();
	void drawGhost();
	void printInfo();
	bool GetPacman();
	bool Collision(int PacX, int PacY, int BallW, int BallH);
	
	void chase(int x, int y, int field[20][20]);
	void chase2(int x, int y, int field[20][20]);
	void chase3(int x, int y, int field[20][20]);
	void chase4(int x, int y, int field[20][20]);

private:
	int hitwall;
	int xpos;
	int ypos;
	char color;
	int direction;
	bool dead;
	int speed;
};

//it helps you be more lazy for the one to move things
enum keyz { KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN };
enum direction { RIGHT, LEFT, UP, DOWN };

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
		0,0,0,0,1,1,1,0,1,0,0,1,1,1,1,1,1,1,0,0,
		1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
		1,0,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,
		1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,
		1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		0,0,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,1,0,1,
		1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
		1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1 };

	//print to testing window
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			cout << level[i][j];
		}
		cout << endl;
	}
	ALLEGRO_DISPLAY *display =  NULL;
	ALLEGRO_EVENT_QUEUE *event_queue =  NULL;
	ALLEGRO_TIMER *timer =  NULL;
	ALLEGRO_BITMAP *PacMan =  NULL;
	ALLEGRO_BITMAP *wall =  NULL;
	ALLEGRO_BITMAP *food =  NULL;
	float PacMan_dx =  - 4.0, PacMan_dy =  4.0;
	int PacMan_x =  470;
	int PacMan_y =  480;


	bool keyz[4] = { false, false, false, false };
	bool redraw =  true;
	bool doexit =  false;

	al_init();
	al_install_keyboard();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_acodec_addon();

	timer =  al_create_timer(1.0 / FPS);

	display =  al_create_display(SCREEN_W, SCREEN_H);

	PacMan =  al_create_bitmap(PacMan_SIZE, PacMan_SIZE);
	al_set_target_bitmap(PacMan);
	al_clear_to_color(al_map_rgb(255, 0, 255));

	wall =  al_create_bitmap(50, 50);
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(0, 0, 255));

	food =  al_create_bitmap(20, 20);
	al_set_target_bitmap(food);
	al_clear_to_color(al_map_rgb(255, 255, 0));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue =  al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	pac b1;
	pac b2;
	pac b3;
	pac b4;

	b1.initGhost(120, 110, 'b', 1, 1, 4);
	b2.initGhost(350, 110, 'r', 1, 1, 4);
	b3.initGhost(500, 500, 'g', 1, 1, 4);
	b4.initGhost(500, 800, 's', 1, 1, 4);

	//you need 3 more

	while (!doexit)
	{

		if (PacMan_y < 0)
			PacMan_y = 1000;
		if (PacMan_y > 1000)
			PacMan_y = 0;
		cout << PacMan_x << " , " << PacMan_y << endl;
		//cout << "game loop" << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type ==  ALLEGRO_EVENT_TIMER) {
			//cout << "timer section" << endl;
			if (keyz[KEY_UP] && !wallCollide(PacMan_x, PacMan_y, UP, level)) {
				PacMan_y -=  4.0;

			}

			if (keyz[KEY_DOWN] && !wallCollide(PacMan_x, PacMan_y, DOWN, level)) {
				PacMan_y +=  4.0;
			}

			if (keyz[KEY_LEFT] && !wallCollide(PacMan_x, PacMan_y, LEFT, level)) {
				PacMan_x -=  4.0;
			}

			if (keyz[KEY_RIGHT] && !wallCollide(PacMan_x, PacMan_y, RIGHT, level)) {
				//cout << "moving right" << endl;

				PacMan_x +=  4.0;
			}

			if (level[PacMan_x / 50][PacMan_y / 50] == 0) {
				level[PacMan_x / 50][PacMan_y / 50] = 2;

			}

			b1.chase(PacMan_x, PacMan_y, level);
			b2.chase(PacMan_x, PacMan_y, level);
			b3.chase(PacMan_x, PacMan_y, level);
			b4.chase(PacMan_x, PacMan_y, level);

			if (b1.Collision(PacMan_x, PacMan_y, PACSIZE, PACSIZE) && !b1.isdead()) {
				PacMan_x = 460;
				PacMan_y = 590;
				//add scary sound effec there
				//reduce lives

			}

			if (b2.Collision(PacMan_x, PacMan_y, PACSIZE, PACSIZE) && !b2.isdead()) {
				PacMan_x = 460;
				PacMan_y = 590;
				//add scary sound effec there
				//reduce lives

			}

			if (b3.Collision(PacMan_x, PacMan_y, PACSIZE, PACSIZE) && !b3.isdead()) {
				PacMan_x = 460;
				PacMan_y = 590;
				//add scary sound effec there
				//reduce lives

			}

			if (b4.Collision(PacMan_x, PacMan_y, PACSIZE, PACSIZE) && !b4.isdead()) {
				PacMan_x = 460;
				PacMan_y = 590;
				//add scary sound effec there
				//reduce lives

			}
			
			redraw =  true;
		}
		else if (ev.type ==  ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		else if (ev.type ==  ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keyz[KEY_UP] =  true;
				break;

			case ALLEGRO_KEY_DOWN:
				keyz[KEY_DOWN] =  true;
				break;

			case ALLEGRO_KEY_LEFT:
				keyz[KEY_LEFT] =  true;
				break;

			case ALLEGRO_KEY_RIGHT:
				keyz[KEY_RIGHT] =  true;
				//cout << "right key pressed!" << endl;
				break;
			}
		}

		else if (ev.type ==  ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keyz[KEY_UP] =  false;
				break;

			case ALLEGRO_KEY_DOWN:
				keyz[KEY_DOWN] =  false;
				break;

			case ALLEGRO_KEY_LEFT:
				keyz[KEY_LEFT] =  false;
				break;

			case ALLEGRO_KEY_RIGHT:
				keyz[KEY_RIGHT] =  false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit =  true;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw =  false;

			al_clear_to_color(al_map_rgb(0, 0, 0));
			for (int i =  0; i < 20; i++)
				for (int j =  0; j < 20; j++) {
					if (level[i][j] ==  1)
						al_draw_bitmap(wall, i * 50, j * 50, 0);

					else if (level[i][j] ==  0)
						al_draw_bitmap(food, i * 50 + 15, j * 50 + 15, 0);

				}

			al_draw_bitmap(PacMan, PacMan_x, PacMan_y, 0);
			b1.drawGhost();
			b2.drawGhost();
			b3.drawGhost();
			b4.drawGhost();

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
	int new_x1;
	int new_x2;
	int new_x3;
	int new_y1;
	int new_y2;
	int new_y3;
	if (dir ==  RIGHT) { // Moving Right
							   // Check along the far right side of the sprite, plus 3 (the amount we're moving)
		new_x1 =  x + 3 + PACSIZE;
		new_x2 =  x + 3 + PACSIZE;
		new_x3 =  x + 3 + PACSIZE;
		// Check at three point along that edge
		new_y1 =  y;
		new_y2 =  y + PACSIZE / 2;
		new_y3 =  y + PACSIZE;

		if ((level[new_x1 / 50][new_y1 / 50] ==  1) || (level[new_x2 / 50][new_y2 /50] ==  1) || (level[new_x3 / 50][new_y3 / 50] ==  1)) {
			cout << "right collision" << endl;
			return 1;
		}
		else {
			return 0;

		}
	}
	if (dir ==  UP) {
		new_x1 =  x;
		new_x2 =  x + PACSIZE / 2;
		new_x3 =  x + PACSIZE;
		new_y1 =  y - 3;
		new_y2 =  y - 3;
		new_y3 =  y - 3;

		if ((level[new_x1 / 50][new_y1 / 50] ==  1) || (level[new_x2 / 50][new_y2 /50] ==  1) || (level[new_x3 / 50][new_y3 / 50] ==  1)) {
			cout << "up collision" << endl;
			return 2;
		}
		else {
			return 0;
		}
	}
	if (dir ==  LEFT) {
		new_x1 =  x - 3;
		new_x2 =  x - 3;
		new_x3 =  x - 3;
		new_y1 =  y;
		new_y2 =  y + PACSIZE / 2;
		new_y3 =  y + PACSIZE;

		if ((level[new_x1 / 50][new_y1 / 50] ==  1) || (level[new_x2 / 50][new_y2 /50] ==  1) || (level[new_x3 / 50][new_y3 / 50] ==  1)) {
			cout << "left collision" << endl;
			return 3;
		}
		else {
			return 0;
		}
	}
	if (dir ==  DOWN) {
		new_x1 =  x;
		new_x2 =  x + PACSIZE / 2;
		new_x3 =  x + PACSIZE;
		new_y1 =  y + PACSIZE + 5;
		new_y2 =  y + PACSIZE + 5;
		new_y3 =  y + PACSIZE + 5;

		if ((level[new_x1 / 50][new_y1 / 50] ==  1) || (level[new_x2 / 50][new_y2 /50] ==  1) || (level[new_x3 / 50][new_y3 / 50] ==  1)) {
			cout << "down collision" << endl;
			return 4;
		}
		else {
			return 0;

		}
		return 0;
	}
}//end of wall collide

bool pac::isdead() {
	if (dead == true)
		return true;
	else
		return false;
}

//ghost();

void pac::initGhost(int x, int y, char c, int dir, bool d, int g) {
	color = c;
	direction = dir;
	speed = g;
	xpos = x;
	ypos = y;
	dead = d;
}

void pac::drawGhost() {
	if (color == 'r')
		al_draw_filled_rectangle(xpos, ypos, xpos + 32, ypos + 32, al_map_rgb(200, 30, 0));
	else if (color == 'b')
		al_draw_filled_rectangle(xpos, ypos, xpos + 32, ypos + 32, al_map_rgb(20, 30, 200));
	else if (color == 'g')
		al_draw_filled_rectangle(xpos, ypos, xpos + 32, ypos + 32, al_map_rgb(20, 200, 20));
	else
		al_draw_filled_rectangle(xpos, ypos, xpos + 32, ypos + 32, al_map_rgb(20, 320, 200));
}

void pac::printInfo() {

}

bool pac::Collision(int PacX, int PacY, int BallW, int BallH) {
	//any time the ball's x position is LESS than the paddle's x position plus its width, you've collided
	if ((PacX + BallW > xpos) && (PacX < xpos + PACSIZE) &&
		//AND any time the ball's x position plus its width is greater than the paddle's x position
		//AND repeat for y (you'll need two statements again)...
		(PacY + BallH > ypos) && (PacY < ypos + PACSIZE)
		)
		return true; //collision!
	
	else
		return false; //no collision
}

//chase function. x and y are pacman's position.

void pac::chase(int x, int y, int field[20][20]) {
	
	/* Directions
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << direction << endl;
	if (wallCollide(xpos, ypos, 1, field))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(xpos, ypos, 2, field))
	cout << "there's a wall above me" << endl;
	if (wallCollide(xpos, ypos, 3, field))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(xpos, ypos, 4, field))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up

	if ((direction == LEFT) && !wallCollide(xpos, ypos, UP, field) && y<ypos)
		while (!wallCollide(xpos, ypos, 2, field)) {
			// cout << "trying to move through hole above!" << endl;
			direction = UP;
			ypos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((direction == LEFT) && !wallCollide(xpos, ypos, DOWN, field) && y>ypos)
		while (!wallCollide(xpos, ypos, 4, field)) {
			//  cout << "trying to move through hole below!" << endl;
			direction = DOWN;
			ypos += 4;
			return;
		}

	if (direction == LEFT)//left
		while (!wallCollide(xpos, ypos, LEFT, field)) {
			xpos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((direction == UP) && !wallCollide(xpos, ypos, LEFT, field) && x<xpos)
		while (!wallCollide(xpos, ypos, LEFT, field)) {
			//   cout << "trying to move through hole to left!" << endl;
			direction = LEFT;
			xpos -= 4;
			return;
		}

	//if we're moving up and there's an opening right and pacman is right, move right
	if ((direction == UP) && !wallCollide(xpos, ypos, RIGHT, field) && x>xpos)
		while (!wallCollide(xpos, ypos, 3, field)) {
			// cout << "trying to move through hole to right!" << endl;
			direction = RIGHT;
			xpos += 4;
			return;
		}

	if (direction == UP)//up
		while (!wallCollide(xpos, ypos, 2, field)) {
			ypos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////

	//if we're moving right and there's an opening above and pacman is above, move up
	if ((direction == RIGHT) && !wallCollide(xpos, ypos, UP, field) && y<ypos)
		while (!wallCollide(xpos, ypos, 2, field)) {
			//   cout << "trying to move through hole above!" << endl;
			direction = UP;
			ypos -= 2;
			return;
		}


	//if we're moving right and there's an opening below and pacman is below, move down
	if ((direction == RIGHT) && !wallCollide(xpos, ypos, DOWN, field) && y>ypos)
		while (!wallCollide(xpos, ypos, DOWN, field)) {
			//  cout << "trying to move through hole below!" << endl;
			direction = DOWN;
			ypos += 2;
			return;
		}

	if (direction == RIGHT)//right
		while (!wallCollide(xpos, ypos, RIGHT, field)) {
			xpos += 2;
			return;
		}

	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((direction == DOWN) && !wallCollide(xpos, ypos, LEFT, field) && x<xpos)
		while (!wallCollide(xpos, ypos, LEFT, field)) {
			// cout << "trying to move through hole to left!" << endl;
			direction = LEFT;
			xpos -= 2;
			return;
		}

	//if we're moving up and there's an opening right and pacman is right, move right
	if ((direction == UP) && !wallCollide(xpos, ypos, RIGHT, field) && x>xpos)
		while (!wallCollide(xpos, ypos, RIGHT, field)) {
			//   cout << "trying to move through hole to right!" << endl;
			direction = RIGHT;
			xpos += 2;
			return;
		}

	if (direction == DOWN)//down
		while (!wallCollide(xpos, ypos, DOWN, field)) {
			ypos += 2;
			return;
		}

	///////////////////////////////////////////////////////////////////////////////////////

	//if pacman is above and there's no wall there, move up
	if ((y<ypos) && !wallCollide(xpos, ypos, UP, field)) {
		//  cout << "direction is up" << endl;
		direction = UP;
		return;
	}

	//if pacman is below and there's no wall there, move down
	if ((y>ypos) && !wallCollide(xpos, ypos, DOWN, field)) {
		direction = DOWN;
		//   cout << "direction is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>xpos) && !wallCollide(xpos, ypos, RIGHT, field)) {
		direction = RIGHT;
		//  cout << "direction is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<xpos) && !wallCollide(xpos, ypos, LEFT, field)) {
		direction = LEFT;
		// cout << "direction is left" << endl;
		return;
	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(xpos, ypos, UP, field)) {
		// cout << "direction is up" << endl;
		direction = UP;
		return;
	}

	//if pacman is below and there's no wall there, move down
	if (!wallCollide(xpos, ypos, DOWN, field)) {
		direction = DOWN;
		//cout << "direction is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(xpos, ypos, RIGHT, field)) {
		direction = RIGHT;
		//  cout << "direction is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(xpos, ypos, LEFT, field)) {
		direction = LEFT;
		// cout << "direction is left" << endl;
		return;
	}
}//cout << "x pos " << xpos
