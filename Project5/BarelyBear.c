#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct wlasciwosci_pola {
	bool przeszkoda;
	int  npc;
	int  przejscie_mapy;
	int npc_id;
};
struct dane_npc {
	int nr_npc;
	int pozx;
	int pozy;
	int poz_docelowa_x;
	int poz_docelowa_y;
	int maxx;
	int maxy;
	int minx;
	int miny;
};
typedef struct wlasciwosci_pola wlasciwosci_pola_t;
typedef struct dane_npc dane_npc_t;
void wczytanie_z_pliku(FILE *wyjscie, wlasciwosci_pola_t **dane, int wysokosc, int szerokosc) {
	int i, j;
	int pomoc[3];
	for (j = 0; j < wysokosc; j++) {
		for (i = 0; i < szerokosc; i++) {
			fscanf_s(wyjscie, "%i", &pomoc[0]);
			fscanf_s(wyjscie, "%i", &pomoc[1]);
			fscanf_s(wyjscie, "%i", &pomoc[2]);
			dane[j][i].przeszkoda = (bool)pomoc[0];
			dane[j][i].npc = pomoc[1];
			dane[j][i].przejscie_mapy = pomoc[2];
		}
	}
}
void czytanie_nazw(char *nazwy[][2]) {

	nazwy[0][0] ="miasto.png";
	nazwy[0][1] ="mapa.leafe";
	nazwy[1][0] ="wnetrze.png";
	nazwy[1][1] ="wnetrze.leafe";

}
void czytanie_nazw_npc(char **nazwy) {
	nazwy[0] = "przykladowynpc1.png";
	nazwy[1] = "przykladowynpc2.png";

}
int main() {
	int mapax = 0;
	int mapay = 0;
	int tmp;
	int zasiegnpc = 3;
	int bohaterx = 350;
	int bohatery = 350;
	int misjednostka = 50;
	int pozdocelowa = 350;
	int xdocelowe = mapax;
	int ydocelowe = mapay;
	int wysokosc, szerokosc,i,j,x;
	int aktualna_pozycjax;
	int aktualna_pozycjay;
	int ilosc_npc,ilosc_rysowanych_npc;
	int aktualna_poz_npc_x, aktualna_poz_npc_y;
	int numerbitmapy=0;
	int xspawnu, yspawnu,xstarejpoz=0,ystarejpoz=0;
	char *nazwy_plikow[2][2];
	char *nazwy_npc[2];
	int xszukania, yszukania, szerszukania=16;
	czytanie_nazw(nazwy_plikow);
	czytanie_nazw_npc(nazwy_npc);
	bool zmianamapy=false;
	bool czy_ekwipunek = false;
	bool przemieszczenie = false;
	al_init();
	ALLEGRO_DISPLAY *okno = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *kolejka = NULL;
	ALLEGRO_BITMAP *bohater = NULL;
	ALLEGRO_BITMAP *ekwipunek = NULL;
	wlasciwosci_pola_t** mapadane = NULL;
	dane_npc_t *dane_do_ruchu_npc;
	FILE *dane_do_mapy;
	
	bool przyciski[4] = { false,false,false,false };
	okno = al_create_display(750, 750);
	al_init_image_addon();
	al_install_keyboard();

	kolejka = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	al_register_event_source(kolejka, al_get_timer_event_source(timer));
	al_register_event_source(kolejka, al_get_keyboard_event_source());
	bool zakonczenie=false;
	
	while (!zakonczenie) {
		ALLEGRO_BITMAP **NPC = NULL;
		ALLEGRO_BITMAP *mapa = NULL;
		zmianamapy = false;
		fopen_s(&dane_do_mapy, nazwy_plikow[numerbitmapy][1], "r");
		fscanf_s(dane_do_mapy, "%i", &wysokosc);
		fscanf_s(dane_do_mapy, "%i", &szerokosc);
		fscanf_s(dane_do_mapy, "%i", &ilosc_npc);
		fscanf_s(dane_do_mapy, "%i", &xspawnu);
		fscanf_s(dane_do_mapy, "%i", &yspawnu);
		//zmiana miejsc pojawiania siê potrzebna do przejsc miedzy mapami
		if (numerbitmapy == 0) {
			mapax = xstarejpoz*misjednostka*-1;
			mapay = ystarejpoz*misjednostka*-1;
		}
		else {
			mapax = xspawnu*misjednostka*-1;
			mapay = yspawnu*misjednostka*-1;
		}
		//ustawienie miejsc docelowych na obecna pozycje bohatera na mapie
		xdocelowe = mapax;
		ydocelowe = mapay;
		mapadane = (wlasciwosci_pola_t **)malloc(wysokosc * sizeof(wlasciwosci_pola_t*));
		for (i = 0; i < wysokosc; i++) {
			mapadane[i] = (wlasciwosci_pola_t*)malloc(szerokosc * sizeof(wlasciwosci_pola_t));
		}
		if (ilosc_npc != 0) {
			NPC = (ALLEGRO_BITMAP**)malloc(ilosc_npc * sizeof(ALLEGRO_BITMAP*));
		}
		wczytanie_z_pliku(dane_do_mapy, mapadane, wysokosc, szerokosc);
		ilosc_rysowanych_npc = 0;
		//zliczenie ilosci rysowanych npc
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				if (mapadane[i][j].npc != 0) {
					ilosc_rysowanych_npc++;
				}
			}
		}
		//deklaracja pamieci na zmienne portzebne do przemieszczanie npc
		dane_do_ruchu_npc = (dane_npc_t*)malloc(ilosc_rysowanych_npc * sizeof(dane_npc_t));
		//zerowanie npc.id na mapie
		x = 1;
		for (i = 0; i < wysokosc; i++) {
			for (j = 0; j < szerokosc; j++) {
				if (mapadane[i][j].npc == 0) {
					mapadane[i][j].npc_id = 0;
				}
				else
				{
					mapadane[i][j].npc_id = x;
					x++;
					tmp = x - 2;//arg
					dane_do_ruchu_npc[tmp].pozx = j * misjednostka*-1;
					dane_do_ruchu_npc[tmp].pozy = i * misjednostka*-1;
					dane_do_ruchu_npc[tmp].poz_docelowa_x = dane_do_ruchu_npc[tmp].pozx;
					dane_do_ruchu_npc[tmp].poz_docelowa_y = dane_do_ruchu_npc[tmp].pozy;
					dane_do_ruchu_npc[tmp].nr_npc = mapadane[i][j].npc;
					dane_do_ruchu_npc[tmp].maxx = j*misjednostka*-1 - zasiegnpc*misjednostka;
					dane_do_ruchu_npc[tmp].minx = j*misjednostka*-1 + zasiegnpc*misjednostka;
					dane_do_ruchu_npc[tmp].miny = i*misjednostka*-1 + zasiegnpc*misjednostka;
					dane_do_ruchu_npc[tmp].maxy = i*misjednostka*-1 - zasiegnpc*misjednostka;
				}
			}
		}
		bohater = al_load_bitmap("bohater.png");
		ekwipunek = al_load_bitmap("ekwipunek.png");
		mapa = al_load_bitmap(nazwy_plikow[numerbitmapy][0]);
		for (i = 0; i < ilosc_npc; i++) {
			NPC[i] = al_load_bitmap(nazwy_npc[i]);
		}
		if (numerbitmapy == 0) {
			mapadane[12][36].przejscie_mapy = 2;
		}
		al_start_timer(timer);
		while (!zmianamapy) {
			ALLEGRO_EVENT zdarzenie;
			al_wait_for_event(kolejka, &zdarzenie);
			if (zdarzenie.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (zdarzenie.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					przyciski[0] = true;
					break;
				case ALLEGRO_KEY_DOWN:
					przyciski[1] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					przyciski[2] = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					przyciski[3] = true;
					break;
				}
			}
			if (zdarzenie.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (zdarzenie.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					przyciski[0] = false;
					break;
				case ALLEGRO_KEY_DOWN:
					przyciski[1] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					przyciski[2] = false;
					break;
				case ALLEGRO_KEY_RIGHT:
					przyciski[3] = false;
					break;
				case ALLEGRO_KEY_ESCAPE:
					zakonczenie = true;
					zmianamapy = true;
					break;
				case ALLEGRO_KEY_E:
					if (czy_ekwipunek == false) {
						czy_ekwipunek = true;
					}
					else
					{
						czy_ekwipunek = false;
					}
					break;
				}
			}
			aktualna_pozycjax = (mapax - bohaterx)*-1/misjednostka;
			aktualna_pozycjay = (mapay - bohatery)*-1/misjednostka;
				if (ydocelowe == mapay && przyciski[0] == true && aktualna_pozycjay > yspawnu) {
					if (mapadane[aktualna_pozycjay - 1][aktualna_pozycjax].przeszkoda == false && mapadane[aktualna_pozycjay - 1][aktualna_pozycjax].npc_id==0) {
						ydocelowe += misjednostka;
					}
				}
				if (ydocelowe == mapay && przyciski[1] == true && aktualna_pozycjay < wysokosc) {
					if (mapadane[aktualna_pozycjay + 1][aktualna_pozycjax].przeszkoda == false && mapadane[aktualna_pozycjay + 1][aktualna_pozycjax].npc_id==0) {
						ydocelowe -= misjednostka;
					}
				}
				if (xdocelowe == mapax && przyciski[2] == true && aktualna_pozycjax > xspawnu) {
					if (mapadane[aktualna_pozycjay][aktualna_pozycjax - 1].przeszkoda == false && mapadane[aktualna_pozycjay][aktualna_pozycjax - 1].npc_id==0) {
						xdocelowe += misjednostka;
					}
				}
				if (xdocelowe == mapax && przyciski[3] == true && aktualna_pozycjax < szerokosc) {
					if (mapadane[aktualna_pozycjay][aktualna_pozycjax + 1].przeszkoda == false && mapadane[aktualna_pozycjay][aktualna_pozycjax + 1].npc_id==0) {
						xdocelowe -= misjednostka;
					}
				}
			printf_s("x  %i       y %i   \n",aktualna_pozycjax, aktualna_pozycjay);
			if (mapadane[aktualna_pozycjay][aktualna_pozycjax].przejscie_mapy !=0) {
				zmianamapy = true;
				if (numerbitmapy == 0) {
					xstarejpoz = aktualna_pozycjax-7;
					ystarejpoz = aktualna_pozycjay-7 + 1;
				}
				numerbitmapy = mapadane[aktualna_pozycjay][aktualna_pozycjax].przejscie_mapy-1;
				
				for (i = 0; i < 4; i++) {
					przyciski[i] = false;
				}
				
			}
			if (zdarzenie.type == ALLEGRO_EVENT_TIMER) {
				if (mapay < ydocelowe) {
					mapay += 2;
				}
				if (mapay > ydocelowe) {
					mapay -= 2;
				}
				if (mapax < xdocelowe) {
					mapax += 2;
				}
				if (mapax > xdocelowe) {
					mapax -= 2;
				}
			}
			al_draw_bitmap(mapa, mapax, mapay, 0);
			xszukania = (mapax / 50)*-1;
			yszukania = (mapay / 50)*-1;
			srand(time(NULL));
			for (i =yszukania; i <yszukania+szerszukania; i++) {
				for (j = xszukania; j < xszukania + szerszukania; j++) {
					if (i >= yspawnu && j >= xspawnu && j < szerokosc &&i < wysokosc) {
						if (mapadane[i][j].npc_id != 0) {
							tmp = mapadane[i][j].npc_id - 1;
							al_draw_bitmap(NPC[dane_do_ruchu_npc[tmp].nr_npc], dane_do_ruchu_npc[tmp].pozx*-1 + mapax, dane_do_ruchu_npc[tmp].pozy*-1 + mapay, 0);
							if (dane_do_ruchu_npc[tmp].pozx == dane_do_ruchu_npc[tmp].poz_docelowa_x && dane_do_ruchu_npc[tmp].pozy == dane_do_ruchu_npc[tmp].poz_docelowa_y) {
								if (dane_do_ruchu_npc[tmp].pozx / misjednostka*-1 != j || dane_do_ruchu_npc[tmp].pozy / misjednostka*-1 != i) {
									x=mapadane[dane_do_ruchu_npc[tmp].pozy / misjednostka*-1][dane_do_ruchu_npc[tmp].pozx / misjednostka*-1].npc_id;
									mapadane[dane_do_ruchu_npc[tmp].pozy / misjednostka*-1][dane_do_ruchu_npc[tmp].pozx / misjednostka*-1].npc_id = mapadane[i][j].npc_id;
									mapadane[i][j].npc_id= x;
								}
								x = rand() % 4;
								aktualna_poz_npc_x = dane_do_ruchu_npc[tmp].pozx / misjednostka*-1;
								aktualna_poz_npc_y = dane_do_ruchu_npc[tmp].pozy / misjednostka*-1;
								switch (x) {
								case 0:
									if (aktualna_poz_npc_x > xspawnu) {
										if (mapadane[aktualna_poz_npc_y][aktualna_poz_npc_x - 1].przeszkoda == false && mapadane[aktualna_poz_npc_y][aktualna_poz_npc_x - 1].npc_id==0 &&(aktualna_pozycjax!=aktualna_poz_npc_x-1 || aktualna_pozycjay!=aktualna_poz_npc_y)) {
											dane_do_ruchu_npc[tmp].poz_docelowa_x += misjednostka;
										}
									}
									break;
								case 1:
									if (aktualna_poz_npc_x  < szerokosc) {
										if (mapadane[aktualna_poz_npc_y][aktualna_poz_npc_x + 1].przeszkoda == false && mapadane[aktualna_poz_npc_y][aktualna_poz_npc_x + 1].npc_id==0 && (aktualna_pozycjax != aktualna_poz_npc_x + 1 || aktualna_pozycjay != aktualna_poz_npc_y)) {
											dane_do_ruchu_npc[tmp].poz_docelowa_x -= misjednostka;
										}
									}
									break;
								case 2:
									if (aktualna_poz_npc_y > yspawnu) {
										if (mapadane[aktualna_poz_npc_y - 1][aktualna_poz_npc_x].przeszkoda == false && mapadane[aktualna_poz_npc_y - 1][aktualna_poz_npc_x].npc_id==0 && (aktualna_pozycjax != aktualna_poz_npc_x || aktualna_pozycjay != aktualna_poz_npc_y-1)) {
											dane_do_ruchu_npc[tmp].poz_docelowa_y += misjednostka;
										}
									}
									break;
								case 3:
									if (aktualna_poz_npc_y < wysokosc) {
										if (mapadane[aktualna_poz_npc_y + 1][aktualna_poz_npc_x].przeszkoda == false && mapadane[aktualna_poz_npc_y + 1][aktualna_poz_npc_x].npc_id==0 && (aktualna_pozycjax != aktualna_poz_npc_x || aktualna_pozycjay != aktualna_poz_npc_y+1)) {
											dane_do_ruchu_npc[tmp].poz_docelowa_y -= misjednostka;
										}
									}
									break;

								}
							}
							if (zdarzenie.type == ALLEGRO_EVENT_TIMER) {
								if (dane_do_ruchu_npc[tmp].pozx < dane_do_ruchu_npc[tmp].poz_docelowa_x) {
									dane_do_ruchu_npc[tmp].pozx += 1;
								}
								if (dane_do_ruchu_npc[tmp].pozx > dane_do_ruchu_npc[tmp].poz_docelowa_x) {
									dane_do_ruchu_npc[tmp].pozx -= 1;
								}
								if (dane_do_ruchu_npc[tmp].pozy < dane_do_ruchu_npc[tmp].poz_docelowa_y) {
									dane_do_ruchu_npc[tmp].pozy += 1;
								}
								if (dane_do_ruchu_npc[tmp].pozy > dane_do_ruchu_npc[tmp].poz_docelowa_y) {
									dane_do_ruchu_npc[tmp].pozy -= 1;
								}
							}
						}
					}
				}
			}
			al_draw_bitmap(bohater, bohaterx, bohatery, 0);
			if (czy_ekwipunek == true) {
				al_draw_bitmap(ekwipunek, 200, 200, 0);
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		fclose(dane_do_mapy);
		free(dane_do_ruchu_npc);
		for (i = 0; i < wysokosc; i++) {
			free(mapadane[i]);
		}
		free(mapadane);
		if (ilosc_npc != 0) {
			for (i = 0; i < ilosc_npc; i++) {
				al_destroy_bitmap(NPC[i]);
			}
			free(NPC);
		}
		al_destroy_bitmap(mapa);
	}
	al_destroy_bitmap(ekwipunek);
	al_destroy_bitmap(bohater);
	al_destroy_display(okno);
	al_destroy_event_queue(kolejka);
	

	return 0;
}