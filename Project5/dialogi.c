#include <stdio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "dialogi.h"
void dialogi(int rodzaj_npc, int* nr_odp,int *rozmowa, int zad,float czas,bool *wziecie_czasu,bool *wybor) {
	al_init();
	al_init_font_addon();
	ALLEGRO_FONT *czcionka;
	//zadanie 1;
	czcionka=al_load_ttf_font("data/czcionka/kleptocracy titling rg.ttf", 24, 0);
	if (rodzaj_npc == 1 && zad ==1) {
		al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "odejdz nie rozmawiam z nieznajomymi");
		if (czas > 3) {
			*rozmowa = 0;
		}
	}
	if (rodzaj_npc == 2 && zad == 1) {
		if (*nr_odp == 0 && czas < 3) {
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "witaj moge ci w czyms pomoc");
		}
		if (czas > 3 && *nr_odp == 0) {
			*wybor = true;
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Nie wiem gdzie jestem");
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "Jestem Glodny");
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 620, 0, "Nie mam gdzie spac");
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 650, 0, "juz wszystko wiem");
		}
		if (*nr_odp == 1 && czas < 3) {
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "Jestes w krainie misiow, nie wiem jak tu sie znalazles");
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "ani dlaczego tutaj jestes");
		}
		if ((*nr_odp == 1 || *nr_odp == 2 || *nr_odp == 3) && czas > 3) {
			*nr_odp = 0;
		}
		if (*nr_odp == 2 && czas < 3) {
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "dam ci cos do jedzenia jezeli mi pomozesz.");
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "Prosze przynies mi wiadro wody");
		}
		if (*nr_odp == 3 && czas < 3) {
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "sproboj zaptac w karczmie moze tam maja jakies ");
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 590, 0, "wolne lozka");
		}
		if (*nr_odp == 4 && czas < 3) {
			al_draw_text(czcionka, al_map_rgb(0, 0, 0), 200, 560, 0, "do zobaczenia :)");
		}
		if (*nr_odp == 4 && czas > 3) {
			*rozmowa = 0;
		}
	}
	al_destroy_font(czcionka);
}