#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

enum KEYS{UP, DOWN, LEFT, RIGHT};

int main(void)
{
	//variables
	int width = 640;
	int height = 480;
	int FPS = 60;

	bool done = false;
	int pos_x = 0;
	int pos_y = 0;

	bool keys[4] = {false, false, false, false};

	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *image = NULL;

	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(width, height);			//create our display object

	if (!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();

	image = al_load_bitmap("mario.png");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			}
		}	
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch(ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					keys[UP] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					keys[DOWN] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					keys[LEFT] = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					keys[RIGHT] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
				}
			}
		else if(ev.type == ALLEGRO_EVENT_TIMER)
			{
				pos_y -= keys[UP] * 10;
				pos_y += keys[DOWN] * 10;
				pos_x -= keys[LEFT] * 10;
				pos_x += keys[RIGHT] * 10;

				al_draw_bitmap(image, pos_x, pos_y, 0);
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));	
			}
	}
	al_destroy_bitmap(image);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}